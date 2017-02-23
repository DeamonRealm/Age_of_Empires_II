#include "j1ClusterAbstraction.h"

Cluster::Cluster(int posX, int posY, int width, int height, int row, int column, int id):posX(posX),posY(posY),width(width), height(height),row(row),column(column),id(id)
{
}

Cluster::~Cluster()
{
}

j1ClusterAbstraction::j1ClusterAbstraction(j1Map * m, uint clusterSize):clusterSize(clusterSize)
{
	if (m->CreateWalkabilityMap(width, height, &map))
		SetMap(width, height, map);
	CreateClusters();
}

j1ClusterAbstraction::~j1ClusterAbstraction()
{
	RELEASE_ARRAY(map);
}
void j1ClusterAbstraction::CreateClusters()
{
	int row = 0, column = 0, clusterID = 0;
	int clusterW = 0, clusterH = 0;
	for (int i = 0; i < height; i += clusterSize) {
		column = 0;
		for (int j = 0; j < width; j += clusterSize) {
			clusterW = MIN(clusterSize, width - j);
			clusterH = MIN(clusterSize, height - i);
			Cluster cluster(j, i, clusterW, clusterH, row, column, clusterID++);
			AddCluster(cluster);
			if (i > 0 && i < width) {
				// (i = 0 clusterH-1 = 9 )== 10
				//
				CreateEntryHorizontal(j, j + clusterW -1, i - 1, row - 1, column);
			}
			if (j > 0 && j < height) {
				CreateEntryVertical(i, i + clusterH -1, j - 1, row, column - 1);
			}

			column++;
		}
		row++;
	}
}
void j1ClusterAbstraction::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width*height];
	memcpy(map, data, width*height);
}

void j1ClusterAbstraction::AddCluster(Cluster add)
{
	clusters.push_back(add);
}

bool j1ClusterAbstraction::IsWalkable(int x, int y) const
{
		bool ret = false;
		iPoint pos(x, y);
		uchar t = GetTileAt(pos);
		if (t != INVALID_WALK_CODE && t > 0) {
			
				ret = true;
			
		}
		return ret;
	}

// Utility: return true if pos is inside the map boundaries
bool j1ClusterAbstraction::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width &&
		pos.y >= 0 && pos.y <= (int)height);
}


// Utility: return the walkability value of a tile
uchar j1ClusterAbstraction::GetTileAt(const iPoint& pos) const
{
	if (CheckBoundaries(pos))
		return map[(pos.y*width) + pos.x];

	return INVALID_WALK_CODE;
}

void j1ClusterAbstraction::CreateEntryHorizontal(int start, int end, int y, int row, int column)
{
	int begin = 0;
	for (int i = start; i < end; i++) {
		while ((i <= end) && !IsWalkable(i,y) && !IsWalkable(i,y+1))
		{
			i++;
		}
		if (i > end)
			return;

		begin = i;
		while ((i <= end) && IsWalkable(i, y) && IsWalkable(i, y + 1))
		{
			i++;
		}
		if ((i - begin) >= MAX_ENTRY_NUM){
			Entry entry1(begin, y, -1, -1, row, column,1, HORIZONTAL);
			entrys.push_back(entry1);
			Entry entry2 (i-1, y, -1, -1, row, column,1, HORIZONTAL);
			entrys.push_back(entry2);

		}
		else {
			Entry entry(((i-1)+begin)*0.5, y, -1, -1, row, column, 1, HORIZONTAL);
			entrys.push_back(entry);
		}
	}
}

void j1ClusterAbstraction::CreateEntryVertical(int start, int end, int x, int row, int column)
{
	int begin = 0;
	for (int i = start; i < end; i++) {
		while ((i <= end) && !IsWalkable(x,i) && !IsWalkable(x+1,i))
		{
			i++;
		}
		if (i > end)
			return;

		begin = i;
		while ((i <= end) && IsWalkable(x, i) && IsWalkable(x + 1, i))
		{
			i++;
		}
		if ((i - begin) >= MAX_ENTRY_NUM) {
			Entry entry1(x,begin, -1, -1, row, column, 1, VERTICAL);
			entrys.push_back(entry1);
			Entry entry2(x,i - 1, -1, -1, row, column, 1, VERTICAL);
			entrys.push_back(entry2);

		}
		else {
			Entry entry(x,((i - 1) + begin)*0.5, -1, -1, row, column, 1, VERTICAL);
			entrys.push_back(entry);
		}
	}
}

int Graph::AddNode(Node * node)
{
	if (node) {
		
		nodes.push_back(node);
		//pushback first because we can't get the size if the size is 0
		node->nodeNum = nodes.size()-1;
		return node->nodeNum;
	}
	//error
	return -1;
}

Edge::Edge(int nodeNum1, int nodeNum2, int distance) : nodeNum1(nodeNum1),nodeNum2(nodeNum2),distance(distance)
{

}
Edge::~Edge()
{
}



Entry::Entry(int posX, int posY, int clusterID1, int clusterID2, int row, int column, int lenght, Orientation orientation): posX(posX),posY(posY),clusterID1(clusterID1),clusterID2(clusterID2),row(row),column(column),lenght(lenght),orientation(orientation)
{
}

Entry::~Entry()
{
}
