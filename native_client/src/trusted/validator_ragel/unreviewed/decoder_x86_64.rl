/*
 * Copyright (c) 2011 The Native Client Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "native_client/src/include/elf32.h"
#include "native_client/src/shared/utils/types.h"
#include "native_client/src/trusted/validator_ragel/unreviewed/decoding.h"

#include "native_client/src/trusted/validator_ragel/gen/decoder_x86_64_instruction_consts.h"

%%{
  machine x86_64_decoder;
  alphtype unsigned char;
  variable p current_position;
  variable pe end_of_data;
  variable eof end_of_data;
  variable cs current_state;

  include byte_machine "byte_machines.rl";

  include prefix_actions
    "native_client/src/trusted/validator_ragel/unreviewed/parse_instruction.rl";
  include prefixes_parsing
    "native_client/src/trusted/validator_ragel/unreviewed/parse_instruction.rl";
  include rex_actions
    "native_client/src/trusted/validator_ragel/unreviewed/parse_instruction.rl";
  include rex_parsing
    "native_client/src/trusted/validator_ragel/unreviewed/parse_instruction.rl";
  include vex_actions_amd64
    "native_client/src/trusted/validator_ragel/unreviewed/parse_instruction.rl";
  include vex_parsing_amd64
    "native_client/src/trusted/validator_ragel/unreviewed/parse_instruction.rl";
  include att_suffix_actions
    "native_client/src/trusted/validator_ragel/unreviewed/parse_instruction.rl";
  include set_spurious_prefixes
    "native_client/src/trusted/validator_ragel/unreviewed/parse_instruction.rl";
  include displacement_fields_actions
    "native_client/src/trusted/validator_ragel/unreviewed/parse_instruction.rl";
  include displacement_fields_parsing
    "native_client/src/trusted/validator_ragel/unreviewed/parse_instruction.rl";
  include modrm_actions_amd64
    "native_client/src/trusted/validator_ragel/unreviewed/parse_instruction.rl";
  include modrm_parsing_amd64
    "native_client/src/trusted/validator_ragel/unreviewed/parse_instruction.rl";
  include operand_actions_amd64
    "native_client/src/trusted/validator_ragel/unreviewed/parse_instruction.rl";
  include immediate_fields_actions
    "native_client/src/trusted/validator_ragel/unreviewed/parse_instruction.rl";
  include immediate_fields_parsing_amd64
    "native_client/src/trusted/validator_ragel/unreviewed/parse_instruction.rl";
  include relative_fields_actions
    "native_client/src/trusted/validator_ragel/unreviewed/parse_instruction.rl";
  include relative_fields_parsing
    "native_client/src/trusted/validator_ragel/unreviewed/parse_instruction.rl";
  include cpuid_actions
    "native_client/src/trusted/validator_ragel/unreviewed/parse_instruction.rl";

  include decode_x86_64 "decoder_x86_64_instruction.rl";

  main := (one_instruction
     @{
        switch (instruction.rm.disp_type) {
          case DISPNONE: instruction.rm.offset = 0; break;
          case DISP8: instruction.rm.offset = (int8_t) *disp; break;
          case DISP16: instruction.rm.offset =
            (uint16_t) (disp[0] + 256U * disp[1]);
            break;
          case DISP32: instruction.rm.offset = (int32_t)
            (disp[0] + 256U * (disp[1] + 256U * (disp[2] + 256U * (disp[3]))));
            break;
          case DISP64: instruction.rm.offset = (int64_t)
            (*disp + 256ULL * (disp[1] + 256ULL * (disp[2] + 256ULL * (disp[3] +
            256ULL * (disp[4] + 256ULL * (disp[5] + 256ULL * (disp[6] + 256ULL *
                                                                 disp[7])))))));
            break;
        }
        switch (imm_operand) {
          case IMMNONE: instruction.imm[0] = 0; break;
          case IMM2: instruction.imm[0] = imm[0] & 0x03; break;
          case IMM8: instruction.imm[0] = imm[0]; break;
          case IMM16: instruction.imm[0] = (uint64_t) (*imm + 256U * (imm[1]));
            break;
          case IMM32: instruction.imm[0] = (uint64_t)
            (imm[0] + 256U * (imm[1] + 256U * (imm[2] + 256U * (imm[3]))));
            break;
          case IMM64: instruction.imm[0] = (uint64_t)
            (imm[0] + 256LL * (imm[1] + 256ULL * (imm[2] + 256ULL * (imm[3] +
            256ULL * (imm[4] + 256ULL * (imm[5] + 256ULL * (imm[6] + 256ULL *
                                                                  imm[7])))))));
            break;
        }
        switch (imm2_operand) {
          case IMMNONE: instruction.imm[1] = 0; break;
          case IMM2: instruction.imm[1] = imm2[0] & 0x03; break;
          case IMM8: instruction.imm[1] = imm2[0]; break;
          case IMM16: instruction.imm[1] = (uint64_t)
            (imm2[0] + 256U * (imm2[1]));
            break;
          case IMM32: instruction.imm[1] = (uint64_t)
            (imm2[0] + 256U * (imm2[1] + 256U * (imm2[2] + 256U * (imm2[3]))));
            break;
          case IMM64: instruction.imm[1] = (uint64_t)
            (*imm2 + 256ULL * (imm2[1] + 256ULL * (imm2[2] + 256ULL * (imm2[3] +
            256ULL * (imm2[4] + 256ULL * (imm2[5] + 256ULL * (imm2[6] + 256ULL *
                                                                 imm2[7])))))));
            break;
        }
        process_instruction(instruction_start, current_position+1, &instruction,
                            userdata);
        instruction_start = current_position + 1;
        SET_DISP_TYPE(DISPNONE);
        SET_IMM_TYPE(IMMNONE);
        SET_IMM2_TYPE(IMMNONE);
        SET_REX_PREFIX(FALSE);
        SET_DATA16_PREFIX(FALSE);
        SET_LOCK_PREFIX(FALSE);
        SET_REPNZ_PREFIX(FALSE);
        SET_REPZ_PREFIX(FALSE);
        SET_BRANCH_NOT_TAKEN(FALSE);
        SET_BRANCH_TAKEN(FALSE);
        SET_VEX_PREFIX2(0xe0);
        SET_VEX_PREFIX3(0x00);
        SET_ATT_INSTRUCTION_SUFFIX(NULL);
        instruction.prefix.data16_spurious = FALSE;
        instruction.prefix.rex_b_spurious = FALSE;
        instruction.prefix.rex_x_spurious = FALSE;
        instruction.prefix.rex_r_spurious = FALSE;
        instruction.prefix.rex_w_spurious = FALSE;
    })*
    $!{ process_error(current_position, userdata);
        result = FALSE;
        goto error_detected;
    };

}%%

%% write data;

#define GET_REX_PREFIX() instruction.prefix.rex
#define SET_REX_PREFIX(P) instruction.prefix.rex = (P)
#define GET_VEX_PREFIX2() vex_prefix2
#define SET_VEX_PREFIX2(P) vex_prefix2 = (P)
#define GET_VEX_PREFIX3() vex_prefix3
#define SET_VEX_PREFIX3(P) vex_prefix3 = (P)
#define SET_DATA16_PREFIX(S) instruction.prefix.data16 = (S)
#define SET_LOCK_PREFIX(S) instruction.prefix.lock = (S)
#define SET_REPZ_PREFIX(S) instruction.prefix.repz = (S)
#define SET_REPNZ_PREFIX(S) instruction.prefix.repnz = (S)
#define SET_BRANCH_TAKEN(S) instruction.prefix.branch_taken = (S)
#define SET_BRANCH_NOT_TAKEN(S) instruction.prefix.branch_not_taken = (S)
#define SET_INSTRUCTION_NAME(N) instruction.name = (N)
#define GET_OPERAND_NAME(N) instruction.operands[(N)].name
#define SET_OPERAND_NAME(N, S) instruction.operands[(N)].name = (S)
#define SET_OPERAND_TYPE(N, S) instruction.operands[(N)].type = (S)
#define SET_OPERANDS_COUNT(N) instruction.operands_count = (N)
#define SET_MODRM_BASE(N) instruction.rm.base = (N)
#define SET_MODRM_INDEX(N) instruction.rm.index = (N)
#define SET_MODRM_SCALE(S) instruction.rm.scale = (S)
#define SET_DISP_TYPE(T) instruction.rm.disp_type = (T)
#define SET_DISP_PTR(P) disp = (P)
#define SET_IMM_TYPE(T) imm_operand = (T)
#define SET_IMM_PTR(P) imm = (P)
#define SET_IMM2_TYPE(T) imm2_operand = (T)
#define SET_IMM2_PTR(P) imm2 = (P)
#define SET_CPU_FEATURE(F)
#define SET_ATT_INSTRUCTION_SUFFIX(S) instruction.att_instruction_suffix = (S)
#define SET_SPURIOUS_DATA16() instruction.prefix.data16_spurious = TRUE;
#define SET_SPURIOUS_REX_B() \
  if (GET_REX_PREFIX() & REX_B) instruction.prefix.rex_b_spurious = TRUE;
#define SET_SPURIOUS_REX_X() \
  if (GET_REX_PREFIX() & REX_X) instruction.prefix.rex_x_spurious = TRUE;
#define SET_SPURIOUS_REX_R() \
  if (GET_REX_PREFIX() & REX_R) instruction.prefix.rex_r_spurious = TRUE;
#define SET_SPURIOUS_REX_W() \
  if (GET_REX_PREFIX() & REX_W) instruction.prefix.rex_w_spurious = TRUE;

enum {
  REX_B = 1,
  REX_X = 2,
  REX_R = 4,
  REX_W = 8
};

enum imm_mode {
  IMMNONE,
  IMM2,
  IMM8,
  IMM16,
  IMM32,
  IMM64
};

int DecodeChunkAMD64(const uint8_t *data, size_t size,
                     ProcessInstructionFunc process_instruction,
                     ProcessDecodingErrorFunc process_error,
                     void *userdata) {
  const uint8_t *current_position = data;
  const uint8_t *end_of_data = data + size;
  const uint8_t *disp = NULL;
  const uint8_t *imm = NULL;
  const uint8_t *imm2 = NULL;
  const uint8_t *instruction_start = current_position;
  uint8_t vex_prefix2 = 0xe0;
  uint8_t vex_prefix3 = 0x00;
  enum imm_mode imm_operand = IMMNONE;
  enum imm_mode imm2_operand = IMMNONE;
  struct Instruction instruction;
  int result = TRUE;

  int current_state;

  SET_DISP_TYPE(DISPNONE);
  SET_IMM_TYPE(IMMNONE);
  SET_IMM2_TYPE(IMMNONE);
  SET_REX_PREFIX(FALSE);
  SET_DATA16_PREFIX(FALSE);
  SET_LOCK_PREFIX(FALSE);
  SET_REPNZ_PREFIX(FALSE);
  SET_REPZ_PREFIX(FALSE);
  SET_BRANCH_NOT_TAKEN(FALSE);
  SET_BRANCH_TAKEN(FALSE);
  SET_ATT_INSTRUCTION_SUFFIX(NULL);
  instruction.prefix.data16_spurious = FALSE;
  instruction.prefix.rex_b_spurious = FALSE;
  instruction.prefix.rex_x_spurious = FALSE;
  instruction.prefix.rex_r_spurious = FALSE;
  instruction.prefix.rex_w_spurious = FALSE;

  %% write init;
  %% write exec;

error_detected:
  return result;
}