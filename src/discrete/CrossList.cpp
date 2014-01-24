//==============================================================================
/*!
 *  \file CrossList.h
 *
 *  \brief CrossList interface
 *
 *  \author chenqinyu
 *          contact:
 *              superthierry14@gmail.com
 *  \date 2013-07
 *
 *  \par
 */
//==============================================================================

#include <strategy_core/discrete/CrossList.h>

CrossListNode::CrossListNode(const UINT i, const UINT j, const REAL e) {
  this->i = i;
  this->j = j;
  this->e = e;
  this->right = NULL;
  this->down = NULL;
}

SingleList::SingleList() {
  this->head = NULL;
  this->tail = NULL;
  this->length = 0;
}

SingleList::~SingleList() {
  CrossListNode *curr = this->head;
  CrossListNode *next;
  while (NULL != curr) {
    next = curr->right; /// Delete the node row by row.
    delete curr;
    curr = next;
  }
  this->head = NULL;
  this->tail = NULL;
}

CrossList::CrossList(const UINT row, const UINT col) {
  this->row = row;
  this->col = col;

  this->rslArray = new SingleList *[this->row];
  for (UINT i = 0; i < this->row; i++) {
    this->rslArray[i] = new SingleList();
    CrossListNode *clnode = new CrossListNode();
    this->rslArray[i]->head = clnode;
    this->rslArray[i]->tail = clnode;
  }

  this->lslArray = new SingleList *[this->col];
  for (UINT j = 0; j < this->col; j++) {
    this->lslArray[j] = new SingleList();
    CrossListNode *clnode = new CrossListNode();
    this->lslArray[j]->head = clnode;
    this->lslArray[j]->tail = clnode;
  }
}

CrossList::~CrossList() {
  for (UINT i = 0; i < this->row; i++) {
    delete this->rslArray[i]; this->rslArray[i] = NULL;
  }
  delete []this->rslArray;  this->rslArray = NULL;

  for (UINT j = 0; j < this->col; j++) {
    delete this->lslArray[j]; this->lslArray[j] = NULL;
  }
  delete []this->lslArray; this->lslArray = NULL;
}

bool CrossList::append(CrossListNode *clnode) {
  bool flag = true;
  if (NULL == clnode) {
    L4C_ERROR("CrossListNode pointer is NULL!");
    flag = false;
    goto end;
  }
  if ((clnode->i >= this->row) or (clnode->j >= this->col)) {
    L4C_ERROR("Sparse element's range [%d, %d] out of defined [%d, %d]!",
              clnode->i, clnode->j, this->row, this->col);
    flag = false;
    goto end;
  }

  this->rslArray[clnode->i]->tail->right = clnode;
  this->rslArray[clnode->i]->tail = clnode;
  this->rslArray[clnode->i]->length++;

  this->lslArray[clnode->j]->tail->down = clnode;
  this->lslArray[clnode->j]->tail = clnode;
  this->lslArray[clnode->j]->length++;

end:
  return flag;
}

SingleList *CrossList::get_row(const UINT row_no) {
  SingleList *result = NULL;
  if (row_no >= this->row or row_no < 0) {
    L4C_WARN("Sparse vector's row range %d out of defined %d!",
             row_no, this->row);
    goto end;
  }
  result = this->rslArray[row_no];

end:
  return result;
}

SingleList *CrossList::get_col(const UINT col_no) {
  SingleList *result = NULL;
  if (col_no >= this->col or col_no < 0) {
    L4C_WARN("Sparse vector's column range %d out of defined %d!",
             col_no, this->col);
    goto end;
  }
  result = this->lslArray[col_no];

end:
  return result;
}

bool CrossList::output_row(const UINT row_no) {
  bool flag = true;
  if (row_no >= this->row or row_no < 0) {
    L4C_WARN("Sparse vector's row range %d out of defined %d!",
             row_no, this->row);
    flag = false;
    goto end;
  }
  else {
    CrossListNode *curr = this->rslArray[row_no]->head->right;
    CrossListNode *next;
    while (NULL != curr) {
      next = curr->right;
      printf("(%d, %d, %f) ", curr->i, curr->j, curr->e);
      curr = next;
    }
  }

end:
  return flag;
}

bool CrossList::output_col(const UINT col_no) {
  bool flag = true;
  if (col_no >= this->col or col_no < 0) {
    L4C_WARN("Sparse vector's column range %d out of defined %d!",
             col_no, this->col);
    flag = false;
    goto end;
  }
  else {
    CrossListNode *curr = this->lslArray[col_no]->head->down;
    CrossListNode *next;
    while (NULL != curr) {
      next = curr->down;
      printf("(%d, %d, %f)\n", curr->i, curr->j, curr->e);
      curr = next;
    }
  }

end:
  return flag;
}

void CrossList::output_all() {
  for (UINT i = 0; i < this->row; i++) {
    this->output_row(i);
    printf("\n");
  }
}
