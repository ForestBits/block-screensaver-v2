api.export();

function blockIs(block, list)
{
	for (var i = 0; i < list.length; ++i)
		if (list[i] == block)
			return true;

	return false;
}

function generateVolcano(centerX, width, height)
{
	var halfSize = math.round(width/2);

	var leftBound = centerX - halfSize >= 0 ? centerX - halfSize : 0;
	var rightBound = centerX + halfSize < blocksWide ? centerX + halfSize : blocksWide - 1;

	for (var x = leftBound; x <= rightBound; ++x)
	{
		var heightPercent;

		if (x != centerX)
			heightPercent = 1 - math.abs(centerX - x)/halfSize;
		else
			heightPercent = 1;

		heightMap[x] = math.floor(heightPercent*height);

		for (var y = 0; y < heightMap[x]; ++y)
			blocks[x][y] = GROUND;
	}

	lavaPoints.push(new Point(centerX, height));

	steamPoints.push(new Point(centerX, height + 1));
}

function generateLavaFlow(x, y)
{
	if (blocks[x][y] == GROUND)
		return;

	blocks[x][y] = LAVA;

	var onGround = blocks[x][y - 1] == GROUND;

	if (!blockIs(blocks[x][y - 1], lavaPathBarriers))
		generateLavaFlow(x, y - 1);

	if (onGround)
	{
		if (inBounds(x - 1, y) && !blockIs(blocks[x - 1][y], lavaPathBarriers))
			generateLavaFlow(x - 1, y);

		if (inBounds(x + 1, y) && !blockIs(blocks[x + 1][y], lavaPathBarriers))
			generateLavaFlow(x + 1, y);
	}
	else
	{
		if (inBounds(x - 1, y) && !blockIs(blocks[x - 1][y], lavaPathBarriers) && blocks[x - 1][y - 1] == GROUND)
			generateLavaFlow(x - 1, y);

		if (inBounds(x + 1, y) && !blockIs(blocks[x + 1][y], lavaPathBarriers) && blocks[x + 1][y - 1] == GROUND)
			generateLavaFlow(x + 1, y);
	}
}

function generateSteam(x, y, angle, steamLeft)
{
	var blockX = math.round(x);
	var blockY = math.round(y);

	if (!inBounds(blockX, blockY) || blocks[blockX][blockY] == LAVA || blocks[blockX][blockY] == GROUND)
		return;

	blocks[blockX][blockY] = STEAM.mixPercent(BACKGROUND, steamLeft);

	steamLeft -= 0.02 + random.getDecimal()*0.05;

	if (steamLeft <= 0)
		return;

	var modifiedAngle = new Angle(angle.value + random.getSign()*random.getDecimal()*30, angle.type);

	var offset = Point.angleOffset(modifiedAngle, 1);

	var nextPosition = new Point(x, y).add(offset);

	generateSteam(nextPosition.x, nextPosition.y, angle, steamLeft);

	var branchChance = 0.1;

	if (random.getDecimal() < branchChance)
	{
		var xOffset = random.getSign()*1;

		if (inBounds(blockX + xOffset, blockY) && blocks[blockX + xOffset][blockY] == BACKGROUND)
			generateSteam(blockX + xOffset, blockY, angle, steamLeft);
	}
}

var GROUND = new Color(107, 107, 107);
var BACKGROUND = new Color(0, 0, 0);
var LAVA = new Color(255, 0, 0);
var STEAM = new Color(200, 200, 200);

var lavaPathBarriers = 
[
	GROUND,
	LAVA
];

setBackgroundBlocks(BACKGROUND);

var minHeight = 2;

var maxHeight = 7;

var maxStep = 1;

var currentHeight = random.getInteger(minHeight, maxHeight);

var heightMap = [];

for (var x = 0; x < blocksWide; ++x)
{
	var step = random.getSign()*random.getInteger(0, maxStep);

	currentHeight += step;

	if (currentHeight < minHeight)
		currentHeight = minHeight;

	if (currentHeight > maxHeight)
		currentHeight = maxHeight;

	heightMap[x] = currentHeight;
}

for (var x = 0; x < blocksWide; ++x)
	for (var y = 0; y < heightMap[x]; ++y)
		blocks[x][y] = GROUND;

var volcanoChance = 0.015;

var lavaPoints = [];

var steamPoints = [];

for (var x = 0; x < blocksWide; ++x)
	if (random.getDecimal() < volcanoChance)
		generateVolcano(x, 20 + random.getInteger(50), 10 + random.getInteger(25));

for (var i = 0; i < lavaPoints.length; ++i)
	generateLavaFlow(lavaPoints[i].x, lavaPoints[i].y);

for (var i = 0; i < steamPoints.length; ++i)
	generateSteam(steamPoints[i].x, steamPoints[i].y, new Angle(80, Angle.DEGREES), 1);