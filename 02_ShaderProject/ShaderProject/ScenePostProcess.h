#ifndef __SCENE_POST_PROCESS_H__
#define __SCENE_POST_PROCESS_H__

#include "SceneBase.hpp"

class ScenePostProcess : public SceneBase
{
public:
	void Init();
	void Uninit();
	void Update(float tick);
	void Draw();

private:
	bool m_isMulti;
	int m_index;
};

#endif // __SCENE_POST_PROCESS_H__