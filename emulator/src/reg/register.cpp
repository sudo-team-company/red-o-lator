#include "register.h"

ModeReg::ModeReg(uint32_t i) {
    value = std::bitset<32>(i);
}
uint8_t ModeReg::fp_round() const {
    return uint8_t(value.to_ulong() & 0xe);
}
uint8_t ModeReg::fp_denorm() const {
    return uint8_t((value.to_ulong() >> 4) & 0xe);
}
bool ModeReg::dx10_clamp() const {
    return value[8];
}
bool ModeReg::ieee() const {
    return value[9];
}
bool ModeReg::lod_clamped() const {
    return value[10];
}
bool ModeReg::debug() const {
    return value[11];
}
uint8_t ModeReg::excp_en() const {
    return uint8_t((value.to_ulong() >> 12) & 0x7f);
}
bool ModeReg::fp16_ovfl() const {
    return value[23];
}
bool ModeReg::pops_packer0() const {
    return value[24];
}
bool ModeReg::pops_packer1() const {
    return value[25];
}
bool ModeReg::disable_perf() const {
    return value[26];
}
bool ModeReg::gpr_idx_en() const {
    return value[27];
}
bool ModeReg::vskip() const {
    return value[28];
}
uint8_t ModeReg::csp() const {
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
bool StatusReg::scc() const {
    return value[0];
}
void StatusReg::scc(bool scc) {
    value[0] = scc;
}

uint8_t StatusReg::spi_prio() const {
    return uint8_t((value.to_ulong() >> 1) & 0x3);
}
uint8_t StatusReg::wave_prio() const {
    return uint8_t((value.to_ulong() >> 3) & 0x3);
}
bool StatusReg::priv() const {
    return value[5];
}
void StatusReg::priv(bool p) {
    value[5] = p;
}
bool StatusReg::trap_en() const {
    return value[6];
}
bool StatusReg::ttrace_en() const {
    return value[7];
}
bool StatusReg::export_rdy() const {
    return value[8];
}
bool StatusReg::execz() const {
    return value[9];
}
bool StatusReg::vccz() const {
    return value[10];
}
bool StatusReg::in_tg() const {
    return value[11];
}
bool StatusReg::in_barrier() const {
    return value[12];
}
bool StatusReg::halt() const {
    return value[13];
}
bool StatusReg::halt(bool bit) {
    return value[13] = bit;
}
bool StatusReg::trap() const {
    return value[14];
}
bool StatusReg::ttrace_cu_en() const {
    return value[15];
}
bool StatusReg::valid() const {
    return value[16];
}
bool StatusReg::ecc_err() const {
    return value[17];
}
bool StatusReg::skip_export() const {
    return value[18];
}
bool StatusReg::perf_en() const {
    return value[19];
}
bool StatusReg::cond_dbg_user() const {
    return value[20];
}
bool StatusReg::cond_dbg_sys() const {
    return value[21];
}
bool StatusReg::allow_replay() const {
    return value[22];
}
bool StatusReg::must_export() const {
    return value[27];
}