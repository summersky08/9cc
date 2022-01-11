#include "9cc.h"

Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *primary();
Node *unary();

// 新しいノードを作成する関数（２種類）
Node *new_node(NodeKind kind){
	Node *node = calloc(1,sizeof(Node));
	node->kind = kind;
	return node;
}

Node *new_binary(NodeKind kind, Node *lhs, Node *rhs) {
  Node *node = new_node(kind);
  node->lhs = lhs;
  node->rhs = rhs;
  return node;
}

Node *new_num(int val){
	Node *node = new_node(ND_NUM);
	node->val = val;
	return node;
}

Node *expr(){
	return equality();
}

// 等式
Node *equality(){
	Node *node = relational();

	for (;;){
		if (consume("=="))
			node = new_binary(ND_EQ, node, relational());
		else if (consume("!="))
			node = new_binary(ND_NEQ, node, relational());
		else
			return node;
	}
}

//比較関数
//長い方から比較する必要がある点に注意
Node *relational(){
	Node *node = add();

	for (;;){
		if (consume("<="))
			node = new_binary(ND_LE, node, add());
		else if (consume(">="))
			node = new_binary(ND_LE, add(), node);
		else if (consume("<"))
			node = new_binary(ND_LT, node, add());
		else if (consume(">"))
			node = new_binary(ND_LT, add(), node);
		else
			return node;
	}
}

// +と-をパースする関数
Node *add(){
	Node *node = mul();

	for (;;){
		if (consume("+"))
			node = new_binary(ND_ADD, node, mul());
		else if (consume("-"))
			node = new_binary(ND_SUB, node, mul());
		else
			return node;
	}
}

Node *mul(){
	Node *node = unary();

	for (;;){
		if (consume("*"))
			node = new_binary(ND_MUL,node,unary());
		else if (consume("/"))
			node = new_binary(ND_DIV,node,unary());
		else
			return node;
	}
}

// 単項演算子の処理
Node *unary(){
	if (consume("+"))
		return unary();
	if (consume("-"))
		return new_binary(ND_SUB, new_num(0),unary());
	return primary();
}

Node *primary(){
	//次のトークンが"("なら、"(" expr ")"のはず
	if (consume("(")){
		Node *node = expr();
		expect(")");
		return node;
	}

	return new_num(expect_number());
}
