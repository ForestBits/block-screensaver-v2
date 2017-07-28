api.export();

var TREE_CURVE_FUNCTIONS = 
[
	function(x)
	{
		return 0.2*x*x;//math.floor(math.pow(x, 1.2));
	},
	function(x)
	{
		return 0.05*x*x;//math.floor(math.pow(x, 1.2));
	}
];

function findInclinePointOfCurve(func, inclineNeeded)
{
	var lastValue = 0;

	for (var i = 0; ; ++i)
		if (func(i) - lastValue >= inclineNeeded)
			return i;
		else
			lastValue = i;
}

function rangeClamp(value, lower, upper)
{
	if (value > upper)
		return upper;
	else if (value < lower)
		return lower;
	else
		return value;
}

function getModifiedColor(color, distance)
{
	var red = rangeClamp(color.red + random.getSign()*distance, 0, 255);
	var green = rangeClamp(color.green + random.getSign()*distance, 0, 255);
	var blue = rangeClamp(color.blue + random.getSign()*distance, 0, 255);

	return new Color(red, green, blue);
}

function generateTreeTrunk(x, y, radius, height, leftCurveFunc, rightCurveFunc)
{
	var leftInclinePoint = findInclinePointOfCurve(leftCurveFunc, 7);
	var rightInclinePoint = findInclinePointOfCurve(rightCurveFunc, 7);

	var heightMap = [];

	var leftBound = x - leftInclinePoint;
	var rightBound = x + rightInclinePoint;

	var unchangedLeftBound = leftBound;

	if (leftBound < 0)
		leftBound = 0;

	if (rightBound >= blocksWide)
		rightBound = blocksWide;

	for (var xPos = leftBound; xPos <= x; ++xPos)
		heightMap[xPos - leftBound] = leftCurveFunc(xPos - unchangedLeftBound);

	for (var xPos = x + 1; xPos <= rightBound; ++xPos)
		heightMap[xPos - leftBound] = rightCurveFunc((x - unchangedLeftBound) - (xPos - x));

	var coreLeftBound = x - radius;
	var coreRightBound = x + radius;

	if (coreLeftBound < 0)
		coreLeftBound = 0;

	if (coreRightBound >= blocksWide)
		coreRightBound = blocksWide - 1;

	for (var xPos = coreLeftBound; xPos <= coreRightBound; ++xPos)
		heightMap[xPos - leftBound] = height;

	for (var i = 0; i < heightMap.length; ++i)
		for (var j = y; j < y + heightMap[i]; ++j)
			if (inBounds(leftBound + i, j))
				blocks[leftBound + i][j] = WOOD;//getModifiedColor(WOOD, 3);
}

function generateLeafCircle(x, y, radius, color)
{
	var biggerRadius = math.ceil(radius);

	var leftBound = x - biggerRadius;
	var rightBound = x + biggerRadius;
	var topBound = y + biggerRadius;
	var bottomBound = y - biggerRadius;

	if (leftBound < 0)
		leftBound = 0;

	if (rightBound >= blocksWide)
		rightBound = blocksWide - 1;

	if (topBound >= blocksHigh)
		topBound = blocksHigh - 1;

	if (bottomBound < 0)
		bottomBound = 0;

	var center = new Point(x, y);

	for (var xPos = leftBound; xPos <= rightBound; ++xPos)
		for (var yPos = bottomBound; yPos <= topBound; ++yPos)
		{
			var distance = center.distance(new Point(xPos, yPos));

			if (distance <= radius)
				blocks[xPos][yPos] = color;//getModifiedColor(color, 10);
		}
}

function generateTreeFoilage(x, y)
{
	var radius = 10;

	var minHeight = 2;
	var maxHeight = 10;

	var leftBound = x - radius;
	var rightBound = x + radius;

	var heightMap = new Array(radius*2 + 1);

	for (var i = 0; i < heightMap.length; ++i)
	{
		if (i < radius)
			heightMap[i] = math.round((i/radius)*(maxHeight - minHeight) + minHeight);
		else if (i == radius)
			heightMap[i] = maxHeight + 1;
		else
			heightMap[i] = math.round(((heightMap.length - i - 1)/radius)*(maxHeight - minHeight) + minHeight);
	}

	for (var xPos = leftBound; xPos <= rightBound; ++xPos)
	{
		if (xPos < 0 || xPos >= blocksWide)
			continue;

		var startHeight = y;
		var height = heightMap[xPos - leftBound];

		print(startHeight + "  " + height);

		for (var yPos = startHeight; yPos < startHeight + height; ++yPos)
			blocks[xPos][yPos] = LEAVES;
	}

	/*var spreadPoint = new Point(x, y);

	var angleRightBound = 25;
	var angleLeftBound = 165;

	var count = random.getInteger(3, 7);

	for (var i = 0; i < count; ++i)
	{
		var center = spreadPoint.add(Point.angleOffset(new Angle(random.getInteger(25, 165), Angle.DEGREES), 1 + random.getDecimal()*7));

		center.x = math.floor(center.x);
		center.y = math.floor(center.y);

		generateLeafCircle(center.x, center.y, random.getInteger(3, 6), LEAVES);
	}()*/
}

function generateTree(x, y, radius, height, leftCurveFunc, rightCurveFunc)
{
	generateTreeTrunk(x, y, radius, height, leftCurveFunc, rightCurveFunc);
	generateTreeFoilage(x, y + height);
}

function generateGround(height)
{
	for (var x = 0; x < blocksWide; ++x)
		for (var y = 0; y < height - 1; ++y)
			blocks[x][y] = DIRT;

	for (var x = 0; x < blocksWide; ++x)
		blocks[x][height - 1] = GRASS;
}


var SKY = new Color(144, 195, 212);
var WOOD = new Color(97, 74, 0);
var LEAVES = new Color(0, 99, 26);
var DIRT = new Color(89, 56, 0);
var GRASS = new Color(21, 97, 0);

//make water flow one at some point

setBackgroundBlocks(SKY);

generateGround(5);
generateTree(25, 5, 3, 25, TREE_CURVE_FUNCTIONS[random.getInteger(TREE_CURVE_FUNCTIONS.length)], TREE_CURVE_FUNCTIONS[random.getInteger(TREE_CURVE_FUNCTIONS.length)]);

//MAKE MINE???