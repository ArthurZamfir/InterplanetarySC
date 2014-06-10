
class Port
{
private:
	bool attached;
	double v;
	SubSystem *s;
public:
	Port();
	~Port(void);

	double getValue();
	void setSubSystem(SubSystem *ss);
	bool isAttached();
	SubSystem* getSubSystem();
};

