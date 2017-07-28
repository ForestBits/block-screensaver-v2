api.export();

function generateFish(x, y, color, facingRight)
{
	var points = 
	[
		new Point(x, y + 1),
		new Point(x, y - 1),
		new Point(x - 1, y),
		new Point(x + 1, y),
		new Point(x - 2, y + 1),
		new Point(x - 2, y - 1),
		new Point(x - 3, y - 1),
		new Point(x - 3, y + 1)
	];

	var clearSpace = true;

	for (var i = 0; i < points.length; ++i)
		if (inBounds(points[i]) && blocks[points[i].x][points[i].y] == SAND)
		{
			clearSpace = false;

			break;
		}

	if (!clearSpace)
		return;

	for (var i = 0; i < points.length; ++i)
	{
		var point = points[i];

		if (!facingRight)
			point.x += (x - point.x)*2;

		if (inBounds(point) && blocks[point.x][point.y] != SAND)
			blocks[point.x][point.y] = color;
	}
}

function generateSchoolOfFish(x, y, color, count, radius)
{
	var facingRight = random.getBoolean();

	var fishPositions = [];

	for (var i = 0; i < count; ++i)
	{
		var angle = new Angle(random.getDecimal()*360, Angle.DEGREES);
		var distance = 1 + random.getDecimal()*(radius - 1);

		var offset = Point.angleOffset(angle, distance);

		var pos = new Point(x, y).add(offset);

		pos.x = math.round(pos.x);
		pos.y = math.round(pos.y);

		var tooClose = false;

		for (var j = 0; j < fishPositions.length; ++j)
		{
			var distance = pos.distance(fishPositions[j]);

			if (distance < 6)
			{
				tooClose = true;

				break;
			}
		}

		if (tooClose)
			continue;

		fishPositions.push(pos);

		generateFish(pos.x, pos.y, color, facingRight);
	}
}

function growSeaweed(x, y)
{
	var currentX = x, currentY = y;

	var maxSeaweedHeight = math.floor(blocksHigh*0.8);

	while (true)
	{
		var growthChance = currentY > maxSeaweedHeight ? 0 : (1 - (currentY/maxSeaweedHeight)*0.2);

		if (random.getDecimal() > growthChance)
			return;

		if (!inBounds(currentX, currentY))
			return;

		if (blocks[currentX][currentY] != WATER)
			return;

		blocks[currentX][currentY] = SEAWEED;

		currentX += random.getInteger(3) - 1;

		var branchChance = growthChance*0.2;

		if (random.getDecimal() < branchChance)
			growSeaweed(currentX + random.getSign(), currentY);

		++currentY;
	}
}

var SAND = new Color(250, 244, 157);
var WATER = new Color(64, 156, 201);
var SEAWEED = new Color(33, 94, 10);

var FISH_COLORS = 
[
	new Color(186, 0, 180),
	new Color(234, 255, 0),
	new Color(255, 128, 0),
	new Color(0, 255, 242),
	new Color(176, 0, 0)
];

setBackgroundBlocks(WATER);

var minHeight = 2;

var maxHeight = 20;

var maxStep = 2;

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
		blocks[x][y] = SAND;

var seaweedChance = 0.05;

for (var x = 0; x < blocksWide; ++x)
	if (random.getDecimal() < seaweedChance)
	{
		growSeaweed(x, heightMap[x]);

		x += 2;
	}

var fishChance = 0.02;

for (var x = 0; x < blocksWide; ++x)
	if (random.getDecimal() < fishChance)
	{
		generateSchoolOfFish(x, heightMap[x] + 5 + random.getInteger(blocksHigh - (heightMap[x] + 5)), FISH_COLORS[random.getInteger(FISH_COLORS.length)], random.getInteger(1, 6), 10);

		x += 20;
	}

