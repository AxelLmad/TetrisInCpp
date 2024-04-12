#ifndef __STATUSS
#define __STATUSS


class Status {

public:
	void change() {
		if (status < maxStatus) {
			++status;
		}
		else {
			status = 0;
		}
	};

	char GetStatus() {
		return status;
	}
protected:
	Status(char maxStatus) {
		this->maxStatus = maxStatus;
	}


private:
	char status{};
	char maxStatus{};
};


class SingleStatus : public Status {
public:
	SingleStatus() :Status(0) {

	}

};

class DoubleStatus : public Status {
public:
	DoubleStatus() : Status(1) {

	}

};

class QuadrupleStatus : public Status {
public:
	QuadrupleStatus() :Status(3) {

	}
};



#endif
