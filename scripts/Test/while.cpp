func void loops()
{
	var int x = -100;
	while (x < 0)
		x += 1;

	var int i = 0;
	while (i != 100) {
		i += 1;
		if (i > 75)
			continue;

		if (i > 50)
			break;
	}

	do {
		i += 1;
	} while(false);
}
