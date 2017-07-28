api.Angle = function(value, type)
{
	this.value = value;
	this.type = type;
};

api.Angle.DEGREES = 0;
api.Angle.RADIANS = 1;

api.Angle.radiansToDegrees = function(radians)
{
	return radians*(180/api.math.PI);
};

api.Angle.degreesToRadians = function(degrees)
{
	return degrees*(api.math.PI/180);
};

api.Angle.prototype.asDegrees = function()
{
	if (this.type == api.Angle.DEGREES)
		return this.value;
	else
		return api.Angle.radiansToDegrees(this.value);
};

api.Angle.prototype.asRadians = function()
{
	if (this.type == api.Angle.RADIANS)
		return this.value;
	else
		return api.Angle.degreesToRadians(this.value);
};