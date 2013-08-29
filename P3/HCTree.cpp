#include "HCTree.hpp"
#include "HCNode.hpp"
#include <queue>
#include <stack>
#include <string>
#include "BitInputStream.hpp"
#include "BitOutputStream.hpp"

/** P3
 *  Implementation of the HCTree class
 *  See: HCTree.hpp
 *  Author: Jay Dey cs100vaj
 *  Author: Joshua Yuen cs100vbc
 */

HCTree::~HCTree()
{
  clear(root);
}


void HCTree::clear(HCNode* node)
{
  if (node == NULL) return;

  clear(node->c0);
  clear(node->c1);
  delete node;
}

void HCTree::build(const vector<int>& freqs) 
{
  priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pQueue;

  for (int i = 0; i < 256; i++){ 
    //convert vector to HCNodes
    //cerr << "beginning build" << endl;
    if( freqs[i] )
    {
      HCNode* temp = new HCNode(freqs[i], (byte)i);
      leaves[i] = temp; //create a new vector to look up the pointers
                      //for those HCNodes based on bit value
      pQueue.push(temp);
      //cerr << "pushed" << endl;
    }
  }

  if ( pQueue.size() == 1)
    pQueue.push(new HCNode(0, 0));

  while(pQueue.size() > 1){
    HCNode* temp1 = pQueue.top();
    pQueue.pop();
    HCNode* temp2 = pQueue.top();
    pQueue.pop();
    HCNode* tempR = new HCNode(temp1->count + temp2->count, 0, temp1, temp2);
    temp1->p = tempR;
    temp2->p = tempR;
    pQueue.push(tempR);
  }
  root = pQueue.top();
}

/*
string HCTree::encodeHelper(byte symbol) const {
  HCNode* temp = leaves[(int)symbol];
  string sTemp;
  while (temp->p){
  if(temp == temp->p->c0){
    sTemp.append("0");
  }
  else{
    sTemp.append("1");
  }
  temp = temp->p;
  }
  return sTemp;
}
*/
void HCTree::encode(byte symbol, BitOutputStream& out) const {
  //string str = encodeHelper(symbol);
  HCNode * temp = leaves[symbol];
  stack<int> code;

  while( temp != root )
  {
    // current is 0 child of parent
    if( temp->p->c0 == temp )
      code.push(0);
    // current is 1 child of parent
    if( temp->p->c1 == temp )
      code.push(1);

    temp = temp->p;
  }

  while( !code.empty() )
  {
    out.writeBit( code.top() );
    code.pop();
  }
  /*
  for (inti = 0; i<str.size(); i++){
    if (str[i] == 1){
      out.writeBit(1);
    }
    else{
      out.writeBit(0);
    }
  }
  */
}


int HCTree::decode(BitInputStream& in) const {
  HCNode* temp = root;
  int bit;
  while ( (temp->c0) && (temp->c1) )
  {
    bit = in.readBit();

    if (bit == 1)
      temp = temp->c1;
    else 
      temp = temp->c0;
  }
  //cerr << temp->symbol;
  return temp->symbol;
   
}
