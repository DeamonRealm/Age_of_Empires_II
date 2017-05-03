#ifndef _RESOURCES_H_
#define _RESOURCES_H_

#include "BaseEntities.h"

///Class Tree ---------------
class Tree : public Resource
{
public: 

	Tree();
	Tree(const Tree& copy);
	~Tree();

private:

	uint cortex = 0;

public:

	//Functionality ---------
	uint	GetCortex()const;
	void	SetCortex(uint val);

	bool	ExtractResources(uint* value);

	//Draw ------------------
	bool	Draw(bool debug);


};
/// -------------------------

#endif // _RESOURCES_H_
