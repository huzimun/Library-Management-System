#include "Controller.h"

int main() {
	cout << "第二个项目正式启动" << endl;
	Controller ctr;
	ctr.start();
	//ctr.managerMenu(); // 因为更改了展示用户函数，所以要先屏蔽展示函数
	return 0;
}