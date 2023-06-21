#ifndef __SCENEVISUAL_H__

#define SCENEVISUAL_H

#include "SceneBase.hpp"

class SceneVisual : public SceneBase
{
public:
	void Init();
	void Uninit();
	void Update(float tick);
	void Draw();
private:
	float m_time;	//	�o�ߎ���
};
#endif // !__SCENEVISUAL_H__
