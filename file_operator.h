#pragma once

int write_customized_block_into_file__file_operator(CustomizedBlock *pstTmp);
int load_block_list__file_operator(Param *pstParam);
int file_operator_insert_block_node_into_chain(
  Param *pstParam, CustomizedBlock *pstTemplateBlock);