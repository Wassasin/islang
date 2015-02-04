#include "common/ast.hpp"
#include "printer.hpp"

int main()
{
	printer p(std::cout);

	{
		ast::type_name tT("T");
		ast::type_name ta("a");
		ast::type_name tb("b");

		ast::type_expr t(tT, {ta, tb});

		p(t, true);
		std::cout << std::endl;
	}

	{
		// data tree a b = Left a | Right b
		ast::type_name ta("a");
		ast::type_name tb("b");

		ast::datadecl_coproduct cl({"Left"}, {{ta}});
		ast::datadecl_coproduct cr({"Right"}, {{tb}});

		ast::datadecl tree({"tree"}, {ta, tb}, {cl, cr});

		// typedef intbintree = tree int int
		ast::type_name tint("int");

		ast::typedefdecl intbintree({"intbintree"}, ast::type_expr({"tree"}, {tint, tint}));

		ast::program prog({tree, intbintree});
		p(prog);
	}

	return 0;
}
