// Copyright (c) 2018 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "source/reduce/operand_to_dominating_id_reduction_opportunity_finder.h"

#include "source/opt/instruction.h"
#include "source/reduce/change_operand_reduction_opportunity.h"

namespace spvtools {
namespace reduce {

using opt::Function;
using opt::IRContext;
using opt::Instruction;

std::vector<std::unique_ptr<ReductionOpportunity>>
OperandToDominatingIdReductionOpportunityFinder::GetAvailableOpportunities(
    IRContext* context) const {
  std::vector<std::unique_ptr<ReductionOpportunity>> result;

  // Go through every instruction in every block, considering it as a potential
  // dominator of other instructions.  We choose this order for two reasons:
  //
  // (1) it is profitable for multiple opportunities to replace the same id x by
  // different dominating ids y and z to be discontiguous, as they are
  // incompatible.
  //
  // (2) We want to prioritise opportunities to replace an id with a more
  // distant dominator.  Intuitively, in a human-readable programming language
  // if we have a complex expression e with many sub-expressions, we would like
  // to prioritise replacing e with its smallest sub-expressions; generalising
  // this idea to dominating ids this roughly corresponds to more distant
  // dominators.
  for (auto& function : *context->module()) {
    for (auto dominating_block = function.begin();
         dominating_block != function.end(); ++dominating_block) {
      for (auto& dominating_inst : *dominating_block) {
        if (dominating_inst.HasResultId() && dominating_inst.type_id()) {
          // Consider replacing any operand with matching type in a dominated
          // instruction with the id generated by this instruction.
          GetOpportunitiesForDominatingInst(
              &result, &dominating_inst, dominating_block, &function, context);
        }
      }
    }
  }
  return result;
}

void OperandToDominatingIdReductionOpportunityFinder::
    GetOpportunitiesForDominatingInst(
        std::vector<std::unique_ptr<ReductionOpportunity>>* opportunities,
        Instruction* candidate_dominator,
        Function::iterator candidate_dominator_block, Function* function,
        IRContext* context) const {
  assert(candidate_dominator->HasResultId());
  assert(candidate_dominator->type_id());
  auto dominator_analysis = context->GetDominatorAnalysis(function);
  // SPIR-V requires a block to precede all blocks it dominates, so it suffices
  // to search from the candidate dominator block onwards.
  for (auto block = candidate_dominator_block; block != function->end();
       ++block) {
    if (!dominator_analysis->Dominates(&*candidate_dominator_block, &*block)) {
      // If the candidate dominator block doesn't dominate this block then there
      // cannot be any of the desired reduction opportunities in this block.
      continue;
    }
    for (auto& inst : *block) {
      // We iterate through the operands using an explicit index (rather
      // than using a lambda) so that we use said index in the construction
      // of a ChangeOperandReductionOpportunity
      for (uint32_t index = 0; index < inst.NumOperands(); index++) {
        const auto& operand = inst.GetOperand(index);
        if (spvIsInIdType(operand.type)) {
          const auto id = operand.words[0];
          auto def = context->get_def_use_mgr()->GetDef(id);
          assert(def);
          if (!context->get_instr_block(def)) {
            // The definition does not come from a block; e.g. it might be a
            // constant.  It is thus not relevant to this pass.
            continue;
          }
          // Sanity check that we don't get here if the argument is a constant.
          assert(!context->get_constant_mgr()->GetConstantFromInst(def));
          if (def->type_id() != candidate_dominator->type_id()) {
            // The types need to match.
            continue;
          }
          if (candidate_dominator != def &&
              dominator_analysis->Dominates(candidate_dominator, def)) {
            // A hit: the candidate dominator strictly dominates the definition.
            opportunities->push_back(
                MakeUnique<ChangeOperandReductionOpportunity>(
                    &inst, index, candidate_dominator->result_id()));
          }
        }
      }
    }
  }
}

std::string OperandToDominatingIdReductionOpportunityFinder::GetName() const {
  return "OperandToDominatingIdReductionOpportunityFinder";
}

}  // namespace reduce
}  // namespace spvtools
