api.export();

function makeWalls()
{
	for (var x = 0; x < blocksWide; ++x)
	{
		blocks[x][0] = WALL;
		blocks[x][blocksHigh - 1] = WALL; 
	}

	for (var y = 0; y < blocksHigh; ++y)
	{
		blocks[0][y] = WALL;
		blocks[blocksWide - 1][y] = WALL;
	}
}



var WALL = new Color(0, 0, 0);
var EMPTY = new Color(255, 255, 255);


setBackgroundBlocks(EMPTY);

makeWalls();