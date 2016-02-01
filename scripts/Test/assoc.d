// Operator associativity test.
// If parsed correctly, both statements in each function
// should produce same result.
func void rightassoc()
{
	x = y = z = calculate();
	x = (y = (z = calculate()));
}
func void leftassoc()
{
	x - y - z - calculate();
	((x - y) - z) - calculate();
}
