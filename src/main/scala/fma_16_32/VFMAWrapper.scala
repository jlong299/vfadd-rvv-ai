/**
  * 13.4 vfmul
  * 13.5
  * 13.6
  * 13.7
  */
package race.vpu.exu.laneexu.fp

import chisel3._
import chisel3.util._
import race.vpu._
import VParams._

// class LaneInput extends Bundle {
//   val uop = new VUop
//   val vs1 = UInt(LaneWidth.W)
//   val vs2 = UInt(LaneWidth.W)
//   val vs3 = UInt(LaneWidth.W)
//   val rs1 = UInt(xLen.W)
// }

// class LaneOutput extends Bundle {
//   val uop = new VUop
//   val vd = UInt(LaneWidth.W)
//   val fflags = Vec(LaneWidth/16, UInt(5.W)) // For eew=32, fflags valid pattern is 0101
// }

class VFMAWrapper extends Module {
  val io = IO(new Bundle {
    val in = Input(ValidIO(new LaneInput))
    val sewIn = Input(new SewFpOH)
    val out = ValidIO(new LaneOutput)
  })

  val vfma0 = Module(new VFMA_16_32)
  val vfma1 = Module(new VFMA_16_32)

  val uop = io.in.bits.uop
  val (vs1, vs2, vs3) = (io.in.bits.vs1, io.in.bits.vs2, io.in.bits.vs3)
  val rs1 = Mux(io.sewIn.is16, Fill(4, io.in.bits.rs1(15, 0)), Fill(2, io.in.bits.rs1(31, 0)))
  
  // Widen case:
  //       32         32
  //    ---------  ---------
  //             |
  //             v   (mux by uopIdx(0))
  //         --------- 32
  //    |            |
  //    v            v
  //  ----   ----   ----   ----
  //   16     16     16     16
  def widen_sel(vs: UInt): UInt = {  // vs is 64 bits
    val (fma1_high, fma1_low, fma0_high, fma0_low) = (WireDefault(vs(63, 48)), vs(47, 32),
                                                      WireDefault(vs(31, 16)), vs(15, 0))
    val vs_32b = Mux(uop.uopIdx(0), vs(63, 32), vs(31, 0))
    when (uop.ctrl.widen) {
      fma0_high := vs_32b(15, 0)
      fma1_high := vs_32b(31, 16)
    }
    Cat(fma1_high, fma1_low, fma0_high, fma0_low)
  }
  
  Seq(vfma0, vfma1).foreach { vfma =>
    vfma.io.valid_in := io.in.valid
    vfma.io.is_bf16 := io.sewIn.isBf16
    vfma.io.is_fp16 := io.sewIn.isFp16
    vfma.io.is_fp32 := io.sewIn.isFp32
    vfma.io.is_widen := uop.ctrl.widen
  }

  val vs1_32b, vs2_32b, vd_32b = Wire(Vec(2, UInt(32.W)))
  for (i <- 0 until 2) {
    vs2_32b(i) := widen_sel(vs2)(i*32+31, i*32)
    vs1_32b(i) := Mux(uop.ctrl.vx, rs1, widen_sel(vs1)(i*32+31, i*32))
    vd_32b(i) := vs3(i*32+31, i*32)
  }

  val funct6 = uop.ctrl.funct6
  val minusAB = funct6(0)
  val minusC = funct6(1) ^ funct6(0)
  val is16In = io.sewIn.is16
  val cIs0 = !uop.ctrl.lsrcVal(2)
  Seq(vfma0, vfma1).zipWithIndex.foreach { case (vfma, i) =>
    when (cIs0) {
      vfma.io.c_in := 0.U
    }.elsewhen (!funct6(2)) { // c_in = (-)vs2
      vfma.io.c_in := inv(vs2_32b(i), minusC, is16In)
    }.elsewhen (is16In && !uop.ctrl.widen) { // c_in = (-)vd && eew=16
      vfma.io.c_in := inv(vd_32b(i), minusC, is16In)
    }.otherwise { // c_in = (-)vd && eew=32
      vfma.io.c_in := inv(vd_32b(i), minusC)
    }

    vfma.io.b_in := inv(vs1_32b(i), minusAB, io.sewIn.is16)
    vfma.io.a_in := Mux(!funct6(2) && !cIs0, vd_32b(i), vs2_32b(i))
  }

  // Output
  io.out.valid := vfma0.io.valid_out
  io.out.bits.vd := Cat(vfma1.io.res_out, vfma0.io.res_out)
  io.out.bits.fflags := VecInit(Seq.fill(LaneWidth/16)(0.U(5.W)))
  io.out.bits.uop := RegEnable(RegEnable(RegEnable(uop, io.in.valid), vfma0.io.valid_S1), vfma1.io.valid_S2)
  
  def inv(fp: UInt, inv_bit: Bool): UInt = {
    Cat(inv_bit ^ fp(fp.getWidth - 1), fp(fp.getWidth - 2, 0))
  }
  def inv(fp32: UInt, inv_bit: Bool, is_16: Bool): UInt = {
    Mux(is_16, 
        Cat(inv(fp32(31, 16), inv_bit), inv(fp32(15, 0), inv_bit)),
        inv(fp32, inv_bit))
  }
}