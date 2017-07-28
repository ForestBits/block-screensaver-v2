api.Color = function(red, green, blue)
{
	this.red = red;
	this.green = green;
	this.blue = blue;
};

api.Color.prototype.toString = function()
{
	return "rgb(" + this.red + ", " + this.green + ", " + this.blue + ")";
};

api.Color.prototype.equals = function(other)
{
	if (!other)
		throw new RangeError("Color equals() comparing with nothing");

	return this.red == other.red && this.green == other.green && this.blue == other.blue;
};

api.Color.prototype.mixPercent = function(color, percentThisColor)
{
	//error checking?

	var red = this.red + (color.red - this.red)*(1 - percentThisColor);
	var green = this.green + (color.green - this.green)*(1 - percentThisColor);
	var blue = this.blue + (color.blue - this.blue)*(1 - percentThisColor);

	return new Color(red, green, blue);
};