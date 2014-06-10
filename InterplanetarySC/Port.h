
class Port
{
private:
	bool attached;
	double v;
public:
	Port();
	~Port(void);

	double getValue();
	void setValue(double value);
	bool isAttached();
	void setAttached();
};

