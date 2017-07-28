api.export();

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

function generateCircle(x, y, radius, color)
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
				blocks[xPos][yPos] = color;
		}
}

function generateGround(minHeight, maxHeight, maxStep)
{
	var currentHeight = random.getInteger(minHeight, maxHeight);

	for (var x = 0; x < blocksWide; ++x)
	{
		var step = random.getSign()*random.getInteger(0, maxStep);

		currentHeight += step;

		if (currentHeight < minHeight)
			currentHeight = minHeight;

		if (currentHeight > maxHeight)
			currentHeight = maxHeight;

		for (var y = 0; y < currentHeight; ++y)
			blocks[x][y] = getModifiedColor(GRAY, 2);
	}
}

function generateCrater(x, radius)
{
	var y = calculateGroundHeight(x);

	generateCircle(x, y, radius, SKY);
}

function generateShootingStar(x, y, angle, spacing)
{
	var points = 
	[
		new Point(x, y),
		new Point(x - 1, y),
		new Point(x + 1, y),
		new Point(x, y + 1),
		new Point(x, y - 1)
	];

	for (var i = 0; i < points.length; ++i)
		if (inBounds(points[i]))
			blocks[points[i].x][points[i].y] = STAR;

	var lastPoint = new Point(x, y);

	while (true)
	{
		lastPoint = lastPoint.add(Point.angleOffset(angle, spacing));

		var roundedPoint = new Point(math.round(lastPoint.x), math.round(lastPoint.y));

		if (!inBounds(roundedPoint))
			break;

		blocks[roundedPoint.x][roundedPoint.y] = STAR_TRAIL;
	}
}

function generateEarth(x, y, radius)
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
			{
				var color;

				switch (random.getInteger(5))
				{
					case 0:
					case 1:
						color = EARTH_BLUE;

						break;

					case 2:
					case 3:
						color = EARTH_GREEN;

						break;

					case 4:
						color = EARTH_WHITE;

						break;
				}

				blocks[xPos][yPos] = color;
			}
		}
}

function generateFlag(x)
{
	var groundHeight = calculateGroundHeight(x);

	var poleHeight = 4;

	for (var y = groundHeight; y < groundHeight + poleHeight; ++y)
		blocks[x][y] = FLAG_POLE;

	var flagRows = 11
	var flagColumns = 13;

	var starBoxWidth = 5;
	var starBoxHeight = 5;

	var flagPosition = new Point(x, groundHeight + poleHeight);

	var starBoxPosition = new Point(x, groundHeight + poleHeight + flagRows - starBoxHeight);

	for (var xPos = flagPosition.x; xPos < flagPosition.x + flagColumns; ++xPos)
		for (var yPos = flagPosition.y; yPos < flagPosition.y + flagRows; ++yPos)
		{
			var color = ((yPos - flagPosition.y) % 2 == 0) ? FLAG_RED : FLAG_WHITE;

			blocks[xPos][yPos] = color;
		}

	for (var xPos = starBoxPosition.x; xPos < starBoxPosition.x + starBoxWidth; ++xPos)
		for (var yPos = starBoxPosition.y; yPos < starBoxPosition.y + starBoxHeight; ++yPos)
		{
			var isStar = ((xPos - starBoxPosition.x) % 2 == 1) && ((yPos - starBoxPosition.y) % 2 == 1);

			blocks[xPos][yPos] = isStar ? FLAG_WHITE : FLAG_BLUE;
		}
}

function calculateGroundHeight(x)
{
	for (var y = 0; ; ++y)
		if (blocks[x][y] == SKY)
			return y;
}

function findGoodFlagSpot(x, searchRadius)
{
	var leftBound = x - searchRadius;
	var rightBound = x + searchRadius;

	if (leftBound < 0)
		leftBound = 0;

	if (rightBound >= blocksWide)
		rightBound = blocksWide - 1;

	var highestValue = 0;
	var highestX = -1;

	for (var xPos = leftBound; xPos <= rightBound; ++xPos)
	{
		var height = calculateGroundHeight(xPos);

		if (height > highestValue)
		{
			highestValue = height;

			highestX = xPos;
		}
	}

	return highestX;
}

function findLunarModuleLandingSpot()
{
	var spaceNeeded = 7;

	var startX = -1;
	var count = 0;
	var currentHeight = -1;

	for (var x = 0; x < blocksWide; ++x)
	{
		var height = calculateGroundHeight(x);

		if (height == currentHeight)
		{
			++count;

			if (count == spaceNeeded)
				return startX;
		}
		else
		{
			currentHeight = height;

			startX = x;

			count = 0;
		}
	}
}

function generateLunarModule(x)
{
	++x;

	var moduleWidth = 7;

	var groundHeight = calculateGroundHeight(x);

	for (var xPos = x; xPos < x + moduleWidth; ++xPos)
		for (var yPos = groundHeight + 1; yPos < groundHeight + 3; ++yPos)
			blocks[xPos][yPos] = MODULE_GOLD;

	blocks[x - 1][groundHeight] = MODULE_GOLD;
	blocks[x + moduleWidth][groundHeight] = MODULE_GOLD;

	generateCircle(x + 3, groundHeight + 5, 3, MODULE_GRAY);
}

var SKY = new Color(0, 0, 0);
var GRAY = new Color(70, 70, 70);
var STAR = new Color(242, 255, 0);
var STAR_TRAIL = new Color(255, 255, 255);
var EARTH_BLUE = new Color(0, 12, 145);
var EARTH_GREEN = new Color(0, 82, 11);
var EARTH_WHITE = new Color(242, 242, 242);
var FLAG_POLE = new Color(200, 200, 200);
var FLAG_WHITE = new Color(255, 255, 255);
var FLAG_RED = new Color(255, 0, 0);
var FLAG_BLUE = new Color(0, 0, 255);
var MODULE_GOLD = new Color(212, 175, 55);
var MODULE_GRAY = new Color(200, 200, 200);

setBackgroundBlocks(SKY);

generateGround(10, 30, 1);

var craterChance = 0.01;

for (var x = 0; x < blocksWide; ++x)
	if (random.getDecimal() < craterChance)
		generateCrater(x, 3 + random.getDecimal()*6);

var starChance = 0.005;

for (var x = 0; x < blocksWide; ++x)
	if (random.getDecimal() < starChance)
	{
		generateShootingStar(x, random.getInteger(35, 45), new Angle(random.getInteger(25, 165), Angle.DEGREES), 6);

		x += 10;
	}

generateEarth(random.getInteger(blocksWide), random.getInteger(40, 45), 3);

var flagSpot = findGoodFlagSpot(random.getInteger(20, blocksWide - 40), 5);

generateFlag(flagSpot);

var landerSpot = findLunarModuleLandingSpot();

if (landerSpot != undefined)
{
	print(landerSpot);

	if (math.abs(landerSpot - flagSpot) > 20)
		generateLunarModule(landerSpot);
	else
		print("kiilled lander - " + math.abs(landerSpot - flagSpot));
}
