api.setBackgroundBlocks = function(color)
{
	for (var x = 0; x < api.blocksWide; ++x)
		for (var y = 0; y < api.blocksHigh; ++y)
			api.blocks[x][y] = color;
};

api.inBounds = function(arg1, arg2)
{
	if (arg1 === undefined)
		throw new Error("inBounds() requires api.Point or number x, number y");

	if (arg1 instanceof api.Point)
	{
		var point = arg1;

		return point.x >= 0 && point.y >= 0 && point.x < api.blocksWide && point.y < blocksHigh;
	}

	if (arg2 === undefined)
		throw new Error("inBounds() requires api.Point or number x, number y");

	if (typeof arg1 != "number")
		throw new Error("Argument 1 is not a number");

	if (typeof arg2 != "number")
		throw new Error("Argument 2 is not a number");

	var x = arg1, y = arg2;

	return x >= 0 && y >= 0 && x < api.blocksWide && y < api.blocksHigh;
};

api.min = function(number1, number2)
{
	if (number1 < number2)
		return number1;
	else
		return number2;
};

api.max = function(number1, number2)
{
	if (number1 > number2)
		return number1;
	else
		return number2;
};