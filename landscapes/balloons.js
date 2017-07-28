api.export();

function generateCircle(x, y, radius, color, blend)
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
				blocks[xPos][yPos] = blend ? color.mixPercent(blocks[xPos][yPos], 0.3) : color;
		}
}

function generateBalloonTail(x, y, length)
{
	var xPos = x, yPos = y;

	while (true)
	{
		if (!inBounds(xPos, yPos))
			return;

		blocks[xPos][yPos] = BALLOON_TAIL;

		var xOffset = random.getInteger(3) - 1;

		xPos += xOffset;

		--yPos;

		--length;

		if (length == 0)
			return;
	}
}

function generateBalloon(x, y, size, tailLength, color)
{
	generateCircle(x, y, size, color, false);
	generateBalloonTail(x, y - math.ceil(size), tailLength);
}

function generateCloud(x, y, length, layers, layersDown)
{
	var originalLength = length;

	for (var layer = 0; layer < layers; ++layer)
	{
		var yPos = y + layer;

		var layerLength = length - layer*3;

		var xPos = x + random.getInteger(originalLength - layerLength);

		for (var layerStart = xPos; xPos < layerStart + layerLength; ++xPos)
			generateCircle(xPos, yPos, 3, CLOUD, true);

		--length;
	}

	for (var layer = 0; layer < layersDown; ++layer)
	{
		var yPos = y - layer;

		var layerLength = length - layer*3;

		var xPos = x + random.getInteger(originalLength - layerLength);

		for (var layerStart = xPos; xPos < layerStart + layerLength; ++xPos)
			generateCircle(xPos, yPos, 3, CLOUD, true);

		--length;
	}
}


var SKY = new Color(144, 195, 212);
var CLOUD = new Color(255, 255, 255);
var BALLOON_TAIL = new Color(0, 0, 0);
var BALLOON_COLORS = 
[
	new Color(255, 0, 0),
	new Color(0, 255, 0),
	new Color(0, 0, 255),
	new Color(255, 255, 0),
	new Color(255, 0, 255),
	new Color(255, 157, 0)
];

var balloonSizes = 
[
	1,
	2,
	2.25,
	3,
	3.25
];

setBackgroundBlocks(SKY);

var balloonChance = 0.05;

for (var x = 0; x < blocksWide; ++x)
	if (random.getDecimal() < balloonChance)
	{
		generateBalloon(x, 10 + random.getInteger(blocksHigh - 11), balloonSizes[random.getInteger(balloonSizes.length)], 7, BALLOON_COLORS[random.getInteger(BALLOON_COLORS.length)]);

		x += 3;
	}

for (var x = random.getInteger(20); x < blocksWide; ++x)
{
	var length = random.getInteger(20, 40);

	generateCloud(x, random.getInteger(10, 15), length, random.getInteger(5, 20), random.getInteger(3, 7));

	x += length + random.getInteger(20);
}