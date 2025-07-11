package race.vpu.exu.laneexu.fp

import chisel3._
import chisel3.util._
import race.vpu._

class FpInfo(value: UInt, fpType: String) {
  require(fpType == "fp16" || fpType == "fp32" || fpType == "bf16", "Invalid fpType")
  private val (expWidth, fracWidth) = fpType match {
    case "fp16" => (5, 10)
    case "bf16" => (8, 7)
    case "fp32" => (8, 23)
    case _ => throw new Exception("Invalid fpType case")
  }
  require(value.getWidth == 1 + expWidth + fracWidth, "Invalid value width")
  private val exp = value.head(1 + expWidth).tail(1)
  private val frac = value(fracWidth - 1, 0)
  val sign = value.head(1).asBool

  val exp_is_0 = exp === 0.U
  val exp_is_all1s = exp.andR
  val frac_is_0 = frac === 0.U

  val isSubnorm = exp_is_0 && !frac_is_0
  val isZero = exp_is_0 && frac_is_0
  val isInf = exp_is_all1s && frac_is_0
  val isNan = exp_is_all1s && !frac_is_0
}