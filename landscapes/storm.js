api.export();

function generateCircle(x, y, radius, color, blendPercent)
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
				blocks[xPos][yPos] = color.mixPercent(blocks[xPos][yPos], blendPercent);
		}
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
			generateCircle(xPos, yPos, 3, CLOUD, 0.15);

		--length;
	}

	for (var layer = 0; layer < layersDown; ++layer)
	{
		var yPos = y - layer;

		var layerLength = length - layer*3;

		var xPos = x + random.getInteger(originalLength - layerLength);

		for (var layerStart = xPos; xPos < layerStart + layerLength; ++xPos)
			generateCircle(xPos, yPos, 3, CLOUD, 0.15);

		--length;
	}
}

function findCloudBottom(x)
{
	for (var y = 0; ; ++y)
		if (blocks[x][y] != SKY)
			return y;
}

function generateRain(rainColumns)
{
	for (var i = 0; i < rainColumns.length; ++i)
	{
		var x = rainColumns[i];
		var y = findCloudBottom(x);

		y -= random.getInteger(0, 5);

		for (; y >= 0; --y)
			if (random.getDecimal() < 0.1)
			{
				blocks[x][y] = RAIN;

				y -= 2;
			}
	}
}

function generateLine(point1, point2, block)
{
	var position = point1;

	var angle = position.angleTo(point2);

	var points = [];

	while (true)
	{
		var roundedPosition = position.round();

		if (roundedPosition.equals(point2))
			break;

		if (!inBounds(roundedPosition))
			break;

		//blocks[roundedPosition.x][roundedPosition.y] = LIGHTNING;
		points.push(roundedPosition);

		position = position.add(Point.angleOffset(angle, 0.5));
	}

	return points;
}

function generateLightningBolt(origin, angle, previousLength, previousBranches, branchyNess)
{
	previousBranches.push(origin);

	var length;

	if (!previousLength)
		length = random.getInteger(15, 40);
	else
		length = previousLength*0.66;

	var magnitude = length;//random.getInteger(20, 40)*reduction;

	var target = origin.add(Point.angleOffset(angle, magnitude)).round();

	var points = generateLine(origin, target, LIGHTNING);

	for (var i = 0; i < points.length; ++i)
		blocks[points[i].x][points[i].y] = LIGHTNING;

	if (random.getDecimal() > branchyNess)
	{print("branch chance");
		var branchAngle = null;

		var tryCount = 0;

		for (var tryCount = 0; tryCount < 4; ++tryCount)
		{
			branchAngle = new Angle(random.getInteger(180 + 35, 360 - 35), Angle.DEGREES);

			if (math.abs(angle.value - branchAngle.value) > 45)
				break;
			else
				branchAngle = null;
		}

		if (!branchAngle)
			print("Couldn't get angle");

		if (!branchAngle)
			return;

		var pointIndex = math.round(points.length/2) + random.getInteger(5);

		if (pointIndex < 0 || pointIndex >= blocksWide){print("point OB");
			return;
		}

		var point = points[pointIndex];

		var tooClose = false;

		for (var i = 0; i < previousBranches.length; ++i)
			if (point.distance(previousBranches[i]) < 10)
			{
				tooClose = true;

				break;
			}

		if (!tooClose)
		{

			generateLightningBolt(point, branchAngle, length, previousBranches, branchyNess);
		}
		else
		{
						print(point);
			print(previousBranches);
		}
	}
}

function generateLightning(lightningPoints)
{
	for (var i = 0; i < lightningPoints.length; ++i)
	{
		var point = lightningPoints[i];

		var angle = new Angle(random.getInteger(180 + 35, 360 - 35), Angle.DEGREES);

		generateLightningBolt(point, angle, null, [], random.getDecimal());
	}
}

var SKY = new Color(0, 33, 54);
var CLOUD = new Color(100, 100, 100);
var RAIN = new Color(0, 0, 255);
var LIGHTNING = new Color(255, 255, 0);


setBackgroundBlocks(SKY);

var rainColumns = [];

var lightningPoints = [];

var lastLightningPoint = -20;
var lightningMinDistance = 40;

for (var x = 0; x < blocksWide; ++x)
	{
		var length = random.getInteger(35, 50);

		generateCloud(x, random.getInteger(35, 50), length, random.getInteger(6, 25), random.getInteger(2, 3));

		for (var i = 0; i < length; ++i)
			if (x + i < blocksWide)
				rainColumns.push(x + i);

		if (x - lastLightningPoint >= lightningMinDistance)
		{
			var xPos = x + random.getInteger(length);

			if (xPos < blocksWide)
			{
				lightningPoints.push(new Point(xPos, findCloudBottom(xPos)));

				lastLightningPoint = xPos;
			}
		}

		x += random.getInteger(length - 10, length - 2);
	}

print(lightningPoints);

generateRain(rainColumns);

generateLightning(lightningPoints);