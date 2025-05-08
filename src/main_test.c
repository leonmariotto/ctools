
void run_tree_test();
void run_list_test();
//void unit_test_lexer();
//void run_node_test();
void	run_unit_test(void)
{
	run_list_test();
	run_tree_test();
	// run_node_test();
	// unit_test_lexer();
}

int main()
{
	run_unit_test();
	return (0);
}
