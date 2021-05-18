//
// Created by Diana Kudaiberdieva
//

#include <stdexcept>

#include "register.h"


ModeReg::ModeReg(uint32_t i) {
    value = std::bitset<32>(i);
}
uint8_t ModeReg::fp_round() {
    return uint8_t(value.to_ulong() & 0xe);
}
uint8_t ModeReg::fp_denorm() {
    return uint8_t((value.to_ulong() >> 4) & 0xe);
}
bool ModeReg::dx10_clamp() {
    return value[8];
}
bool ModeReg::ieee() {
    return value[9];
}
bool ModeReg::lod_clamped() {
    return value[10];
}
bool ModeReg::debug() {
    return value[11];
}
uint8_t ModeReg::excp_en() {
    return uint8_t((value.to_ulong() >> 12) & 0x7f);
}
bool ModeReg::fp16_ovfl() {
    return value[23];
}
bool ModeReg::pops_packer0() {
    return value[24];
}
bool ModeReg::pops_packer1() {
    return value[25];
}
bool ModeReg::disable_perf() {
    return value[26];
}
bool ModeReg::gpr_idx_en() {
    return value[27];
}
bool ModeReg::vskip() {
    return value[28];
}
uint8_t ModeReg::csp() {
    return uint8_t((value.to_ulong() >> 29) & 0xe);
}
void ModeReg::fp_round(uint8_t i) {
    value |= (i & 0xe);
}
void ModeReg::fp_denorm(uint8_t i) {
    value |= (uint32_t(0) | (i & 0xe)) << 4;
}
void ModeReg::dx10_clamp(bool b) {
    value[8] = b;
}
void ModeReg::ieee(bool b) {
    value[9] = b;
}
void ModeReg::lod_clamped(bool b) {
    value[10] = b;
}
void ModeReg::debug(bool b) {
    value[11] = b;
}
void ModeReg::excp_en(uint8_t i) {
    value |= (uint32_t(0) | (i & 0x3f)) << 12;
}
void ModeReg::fp16_ovfl(bool b) {
    value[23] = b;
}
void ModeReg::pops_packer0(bool b) {
    value[24] = b;
}
void ModeReg::pops_packer1(bool b) {
    value[25] = b;
}
void ModeReg::disable_perf(bool b) {
    value[26] = b;
}
void ModeReg::gpr_idx_en(bool b) {
    value[27] = b;
}
void ModeReg::vskip(bool b) {
    value[28] = b;
}
void ModeReg::csp(uint8_t i) {
    value |= (uint32_t(0) | (i & 0xe)) << 29;
}

StatusReg::StatusReg(uint32_t i) {
    value = std::bitset<32>(i);
}
bool StatusReg::scc() {
    return value[0];
}
uint8_t StatusReg::spi_prio() const {
    return uint8_t((value.to_ulong() >> 1) & 0x3);
}
uint8_t StatusReg::wave_prio() const {
    return uint8_t((value.to_ulong() >> 3) & 0x3);
}
bool StatusReg::priv() {
    return value[5];
}
bool StatusReg::trap_en() {
    return value[6];
}
bool StatusReg::ttrace_en() {
    return value[7];
}
bool StatusReg::export_rdy() {
    return value[8];
}
bool StatusReg::execz() {
    return value[9];
}
bool StatusReg::vccz() {
    return value[10];
}
bool StatusReg::in_tg() {
    return value[11];
}
bool StatusReg::in_barrier() {
    return value[12];
}
bool StatusReg::halt() {
    return value[13];
}
bool StatusReg::halt(bool bit) {
    return value[13] = bit;
}
bool StatusReg::trap() {
    return value[14];
}
bool StatusReg::ttrace_cu_en() {
    return value[15];
}
bool StatusReg::valid() {
    return value[16];
}
bool StatusReg::ecc_err() {
    return value[17];
}
bool StatusReg::skip_export() {
    return value[18];
}
bool StatusReg::perf_en() {
    return value[19];
}
bool StatusReg::cond_dbg_user() {
    return value[20];
}
bool StatusReg::cond_dbg_sys() {
    return value[21];
}
bool StatusReg::allow_replay() {
    return value[22];
}
bool StatusReg::must_export() {
    return value[27];
}

int get_register_size(RegisterType registerType) {
    switch (registerType) {
        case PC: return 48;
        case EXEC: return 64;
        case VCC: return 64;
        case FLAT_SCRATCH: return 64;
        case XNACK_MASK: return 64;
        case STATUS: return 32;
        case MODE: return 32;
        case M0: return 32;
        case TRAPSTS: return 32;
        case TBA: return 64;
        case TMA: return 64;
        case TTMP0:
        case TTMP1:
        case TTMP2:
        case TTMP3:
        case TTMP4:
        case TTMP5:
        case TTMP6:
        case TTMP7:
        case TTMP8:
        case TTMP9:
        case TTMP10:
        case TTMP11:
        case TTMP12:
        case TTMP13:
        case TTMP14:
        case TTMP15: return 32;
        case VMCNT: return 6;
        case EXPCNT: return 3;
        case LGKMCNT: return 4;
        case S0:
        case S1:
        case S2:
        case S3:
        case S4:
        case S5:
        case S6:
        case S7:
        case S8:
        case S9:
        case S10:
        case S11:
        case S12:
        case S13:
        case S14:
        case S15:
        case S16:
        case S17:
        case S18:
        case S19:
        case S20:
        case S21:
        case S22:
        case S23:
        case S24:
        case S25:
        case S26:
        case S27:
        case S28:
        case S29:
        case S30:
        case S31:
        case S32:
        case S33:
        case S34:
        case S35:
        case S36:
        case S37:
        case S38:
        case S39:
        case S40:
        case S41:
        case S42:
        case S43:
        case S44:
        case S45:
        case S46:
        case S47:
        case S48:
        case S49:
        case S50:
        case S51:
        case S52:
        case S53:
        case S54:
        case S55:
        case S56:
        case S57:
        case S58:
        case S59:
        case S60:
        case S61:
        case S62:
        case S63:
        case S64:
        case S65:
        case S66:
        case S67:
        case S68:
        case S69:
        case S70:
        case S71:
        case S72:
        case S73:
        case S74:
        case S75:
        case S76:
        case S77:
        case S78:
        case S79:
        case S80:
        case S81:
        case S82:
        case S83:
        case S84:
        case S85:
        case S86:
        case S87:
        case S88:
        case S89:
        case S90:
        case S91:
        case S92:
        case S93:
        case S94:
        case S95:
        case S96:
        case S97:
        case S98:
        case S99:
        case S100:
        case S101: return 32;
        case V0:
        case V1:
        case V2:
        case V3:
        case V4:
        case V5:
        case V6:
        case V7:
        case V8:
        case V9:
        case V10:
        case V11:
        case V12:
        case V13:
        case V14:
        case V15:
        case V16:
        case V17:
        case V18:
        case V19:
        case V20:
        case V21:
        case V22:
        case V23:
        case V24:
        case V25:
        case V26:
        case V27:
        case V28:
        case V29:
        case V30:
        case V31:
        case V32:
        case V33:
        case V34:
        case V35:
        case V36:
        case V37:
        case V38:
        case V39:
        case V40:
        case V41:
        case V42:
        case V43:
        case V44:
        case V45:
        case V46:
        case V47:
        case V48:
        case V49:
        case V50:
        case V51:
        case V52:
        case V53:
        case V54:
        case V55:
        case V56:
        case V57:
        case V58:
        case V59:
        case V60:
        case V61:
        case V62:
        case V63:
        case V64:
        case V65:
        case V66:
        case V67:
        case V68:
        case V69:
        case V70:
        case V71:
        case V72:
        case V73:
        case V74:
        case V75:
        case V76:
        case V77:
        case V78:
        case V79:
        case V80:
        case V81:
        case V82:
        case V83:
        case V84:
        case V85:
        case V86:
        case V87:
        case V88:
        case V89:
        case V90:
        case V91:
        case V92:
        case V93:
        case V94:
        case V95:
        case V96:
        case V97:
        case V98:
        case V99:
        case V100:
        case V101:
        case V102:
        case V103:
        case V104:
        case V105:
        case V106:
        case V107:
        case V108:
        case V109:
        case V110:
        case V111:
        case V112:
        case V113:
        case V114:
        case V115:
        case V116:
        case V117:
        case V118:
        case V119:
        case V120:
        case V121:
        case V122:
        case V123:
        case V124:
        case V125:
        case V126:
        case V127:
        case V128:
        case V129:
        case V130:
        case V131:
        case V132:
        case V133:
        case V134:
        case V135:
        case V136:
        case V137:
        case V138:
        case V139:
        case V140:
        case V141:
        case V142:
        case V143:
        case V144:
        case V145:
        case V146:
        case V147:
        case V148:
        case V149:
        case V150:
        case V151:
        case V152:
        case V153:
        case V154:
        case V155:
        case V156:
        case V157:
        case V158:
        case V159:
        case V160:
        case V161:
        case V162:
        case V163:
        case V164:
        case V165:
        case V166:
        case V167:
        case V168:
        case V169:
        case V170:
        case V171:
        case V172:
        case V173:
        case V174:
        case V175:
        case V176:
        case V177:
        case V178:
        case V179:
        case V180:
        case V181:
        case V182:
        case V183:
        case V184:
        case V185:
        case V186:
        case V187:
        case V188:
        case V189:
        case V190:
        case V191:
        case V192:
        case V193:
        case V194:
        case V195:
        case V196:
        case V197:
        case V198:
        case V199:
        case V200:
        case V201:
        case V202:
        case V203:
        case V204:
        case V205:
        case V206:
        case V207:
        case V208:
        case V209:
        case V210:
        case V211:
        case V212:
        case V213:
        case V214:
        case V215:
        case V216:
        case V217:
        case V218:
        case V219:
        case V220:
        case V221:
        case V222:
        case V223:
        case V224:
        case V225:
        case V226:
        case V227:
        case V228:
        case V229:
        case V230:
        case V231:
        case V232:
        case V233:
        case V234:
        case V235:
        case V236:
        case V237:
        case V238:
        case V239:
        case V240:
        case V241:
        case V242:
        case V243:
        case V244:
        case V245:
        case V246:
        case V247:
        case V248:
        case V249:
        case V250:
        case V251:
        case V252:
        case V253:
        case V254:
        case V255: return 32;
    }
}
