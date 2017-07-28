api.export();

function getRandomAngleInRange(min, max)
{
	return new Angle(random.getInteger(min, max), )
}

function getRandomFireworkColor()
{
	return FIREWORK_COLORS[random.getInteger(FIREWORK_COLORS.length)];
}

function getRandomDistinctFireworkColors(count)
{
	var colorsCopy = FIREWORK_COLORS.slice();

	var colors = [];

	for (var i = 0; i < count; ++i)
	{
		var index = random.getInteger(colorsCopy.length);

		colors.push(colorsCopy[index]);

		colorsCopy.splice(index, 1);
	}

	return colors;
}

function generateFireworkTrail(x, y, angle, spacing, color)
{
	var position = new Point(x, y);

	while (true)
	{
		var roundedPosition = new Point(math.round(position.x), math.round(position.y));

		if (!inBounds(roundedPosition))
			return;
			
		blocks[roundedPosition.x][roundedPosition.y] = color;

		position = position.add(Point.angleOffset(angle, spacing));
	}
}

function BasicFirework(color, radius)
{
	this.color = color;
	this.radius = radius;
}

BasicFirework.prototype.explode = function(x, y)
{
	var center = new Point(x, y);

	var degrees;

	for (degrees = 0; degrees < 360; degrees += 15)
	{
		var point = center.add(Point.angleOffset(new Angle(degrees, Angle.DEGREES), this.radius));

		var roundedPoint = new Point(math.round(point.x), math.round(point.y));

		if (inBounds(roundedPoint))
			blocks[roundedPoint.x][roundedPoint.y] = this.color;
	}
};

function BasicFireworkPattern()
{

}

BasicFireworkPattern.prototype.generateFireworks = function(x)
{
	var position = new Point(x, random.getInteger(20, 40));

	new BasicFirework(getRandomFireworkColor(), 10).explode(position.x, position.y);

	generateFireworkTrail(position.x, position.y, getRandomAngleInRange(295, 255), 10, FIREWORK_TRAIL);

	return x + 12;
};

function AscendingFireworkPattern(startHeight, ascent, spacing, count)
{
	this.startHeight = startHeight;
	this.ascent = ascent;
	this.spacing = spacing;
	this.count = count;
}

AscendingFireworkPattern.prototype.generateFireworks = function(x)
{
	var colors = getRandomDistinctFireworkColors(this.count);

	for (var i = 0; i < this.count; ++i)
	{
		var position = new Point(x + this.spacing*i, this.startHeight + this.ascent*i);

		new BasicFirework(colors[i], 7).explode(position.x, position.y);

		generateFireworkTrail(position.x, position.y, getRandomAngleInRange(295, 255), 10, FIREWORK_TRAIL);
	}

	return x + 10*this.count + 5;
};

function RandomFireworkPattern(height, radius, count)
{
	this.height = height;
	this.radius = radius;
	this.count = count;
}

RandomFireworkPattern.prototype.generateFireworks = function(x)
{
	var colors = getRandomDistinctFireworkColors(this.count);

	var center = new Point(x + 20, random.getInteger(20, 40));

	var furthestRight = -1;

	for (var i = 0; i < this.count; ++i)
	{
		var angle = getRandomAngleInRange(0, 359);
		var magnitude = random.getInteger(5, 15);

		var position = center.add(Point.angleOffset(angle, magnitude));

		if (position.x > furthestRight)
			furthestRight = position.x;

		new BasicFirework(colors[i], random.getInteger(3, 9)).explode(position.x, position.y);

		generateFireworkTrail(position.x, position.y, getRandomAngleInRange(295, 255), 10, FIREWORK_TRAIL);
	}

	return furthestRight + 10;
};

function ColumnFireworkPattern(height, count, initialSize, heightIncrement, radiusIncrement)
{
	this.height = height;
	this.count = count;
	this.initialSize = initialSize;
	this.heightIncrement = heightIncrement;
	this.radiusIncrement = radiusIncrement;
}

ColumnFireworkPattern.prototype.generateFireworks = function(x)
{
	var color = getRandomFireworkColor();

	var radius = this.initialSize;

	for (var i = 0; i < this.count; ++i)
	{
		var position = new Point(x, this.height + i*this.heightIncrement);

		new BasicFirework(color, radius).explode(position.x, position.y);

		generateFireworkTrail(position.x, position.y, new Angle(270, Angle.DEGREES), 10, FIREWORK_TRAIL);

		radius += this.radiusIncrement;
	}

	return x + radius + 2;
};

function getFireworkPattern()
{
	switch (random.getInteger(4))
	{
		case 0:
			return new BasicFireworkPattern();

		case 1:
			return new AscendingFireworkPattern(20, 5, random.getInteger(10, 25), random.getInteger(3, 6));

		case 2:
			return new RandomFireworkPattern(random.getInteger(20, 40), 10, random.getInteger(3, 7));

		case 3:
			return new ColumnFireworkPattern(random.getInteger(5, 15), 4, 5, 10, 3);
	}
}


var BACKGROUND = new Color(0, 0, 0);
var FIREWORK_TRAIL = new Color(255, 255, 255);
var FIREWORK_COLORS =
[
	new Color(255, 0, 0),
	new Color(0, 255, 0),
	new Color(255, 255, 0),
	new Color(0, 255, 255),
	new Color(255, 0, 255),
	new Color(255, 174, 0),
	new Color(183, 0, 255)
];

setBackgroundBlocks(BACKGROUND);

var x = 0;

while (x < blocksWide)
{
	x = getFireworkPattern().generateFireworks(x);

	x += random.getInteger(45, 70);
}