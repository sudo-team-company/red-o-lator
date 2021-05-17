//
// Created by Diana Kudaiberdieva
//

#include <stdexcept>
#include "instruction.h"


#define UNEXPECTED_FORMAT_ASSERT(actual_format) assert((actual_format) == INSTR_FORMAT)

#define BASIC_ASSERTIONS(operands_num, format) \
    UNEXPECTED_FORMAT_ASSERT((format));        \
    assert((operands_num) == OPERANDS_NUMBER)

namespace SOP1 {
#define OPERANDS_NUMBER 2
#define INSTR_FORMAT SOP2_FORMAT

Operand* get_dst(Instruction& instr){
    BASIC_ASSERTIONS(instr.get_operands_count(), get_instr_format(instr.get_instr_key()));
    return instr[0];
}

Operand* get_src0(Instruction& instr) {
    BASIC_ASSERTIONS(instr.get_operands_count(), get_instr_format(instr.get_instr_key()));
    return instr[1];
}

void execute_instr(Instruction& instr) {
    Operand* sdst = get_dst(instr);
    Operand* src0 = get_src0(instr);


}

#undef INSTR_FORMAT
#undef OPERANDS_NUMBER
}

namespace SOP2 {
#define OPERANDS_NUMBER 3
#define INSTR_FORMAT SOP2_FORMAT
Operand* get_dst(Instruction& instr) {
    BASIC_ASSERTIONS(instr.get_operands_count(), get_instr_format(instr.get_instr_key()));
    return instr[0];
}

Operand* get_src0(Instruction& instr) {
    BASIC_ASSERTIONS(instr.get_operands_count(), get_instr_format(instr.get_instr_key()));
    return instr[1];
}

Operand* get_src1(Instruction& instr) {
    BASIC_ASSERTIONS(instr.get_operands_count(), get_instr_format(instr.get_instr_key()));
    return instr[2];
}
#undef OPERANDS_NUMBER
#undef INSTR_FORMAT
}

namespace SOPK {
#define OPERANDS_MAX_NUMBER 3
#define INSTR_FORMAT SOPK_FORMAT

Operand get_dst(const Instruction& instr) {
    UNEXPECTED_FORMAT_ASSERT(get_instr_format(instr.get_instr_key()));
    throw std::runtime_error("Not implemented");
}

//Operand get_src0(const Instruction& instr) {
//    auto operands = instr.get_operands();
//    BASIC_ASSERTIONS
//    return operands[1];
//}
//
//Operand get_src1(const Instruction& instr) {
//    auto operands = instr.get_operands();
//    BASIC_ASSERTIONS
//    return operands[2];
//}
#undef OPERANDS_NUMBER
#undef INSTR_FORMAT
}
