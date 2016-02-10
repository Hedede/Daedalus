
func void test1(var int x, var int y)
{
	// var int i;
	if (x > y) {
		y = x;
	} else {
		x = y;
	}
}

func void test2()
{
	do_nothing();
	do_something(2.0);
	do_something_more("test", "also_test");
}

func void test3()
{
	x = y = z = calculate();
	x = (y = (z = calculate()));
}

func float test4()
{
	a + b * c - (d + e) * f * g + h;
}

func void junk()
{
	{}
	x = 2;
	if (2 + /*** * *** * **** ** ** *** ** * ** * * * **** ** *** ** ** * ** ** ** ** * ** * ** */2) {
		2;
		"zzzz";
	} else {
		test(2,2.0);
	}
	test2(x);
	test3(test4(1/3.0, test2()+1));
}

