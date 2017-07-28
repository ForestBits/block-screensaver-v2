api.Point = function(x, y)
{
	this.x = x;
	this.y = y;
};

api.Point.prototype.distance = function(other)
{
	if (!(other instanceof api.Point))
		throw new RangeError("argument is not a Point");

	return api.getDistance(this.x, this.y, other.x, other.y);
}

api.Point.prototype.add = function(other)
{
	if (!(other instanceof api.Point))
		throw new RangeError("argument is not a Point");

	return new api.Point(this.x + other.x, this.y + other.y);
}

api.Point.prototype.subtract = function(other)
{
	if (!(other instanceof api.Point))
		throw new RangeError("argument is not a Point");

	return new api.Point(this.x - other.x, this.y - other.y);
}

api.Point.prototype.manhattanDistance = function(other)
{
	if (!(other instanceof api.Point))
		throw new RangeError("argument is not a Point");

	return api.getManhattanDistance(this.x, this.y, other.x, other.y);
}

api.Point.prototype.offset = function(other)
{
	if (!(other instanceof api.Point))
		throw new RangeError("argument is not a Point");

	return new api.Point(this.x - other.x, this.y - other.y);
};

api.Point.prototype.angleTo = function(other)
{
	if (!(other instanceof api.Point))
		throw new RangeError("argument is not a Point");

	return new api.Angle(api.math.atan2(other.y - this.y, other.x - this.x), api.Angle.RADIANS);
};

api.Point.prototype.round = function()
{
	return new api.Point(api.math.round(this.x), api.math.round(this.y));
};

api.Point.prototype.equals = function(other)
{
	if (!(other instanceof api.Point))
		throw new RangeError("argument is not a Point");

	return (this.x == other.x && this.y == other.y);
};

api.Point.angleOffset = function(angle, magnitude)
{
	if (!(angle instanceof api.Angle))
		throw new RangeError("angle argument is not an Angle");

	if (isNaN(magnitude))
		throw new RangeError("magnitude argument is not a number");

	var rads = angle.asRadians();

	var xOffset = api.math.cos(rads)*magnitude;
	var yOffset = api.math.sin(rads)*magnitude;

	return new api.Point(xOffset, yOffset);
};