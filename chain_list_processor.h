#pragma once

int find_avaiable_serial__chain_list_processor(Param *pstParam);
int get_block_total_num__chain_list_processor(Param *pstParam);
int build_fast_array__chain_list_processor(Param *pstParam);
int insert_block_element_node_into_chain__chain_list_processor(
  Param *pstParam, BlockElement *pstBlockElementTemplate);
int free_current_block_element__chain_list_processor(Param *pstParam);