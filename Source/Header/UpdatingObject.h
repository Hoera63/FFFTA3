#ifndef UPDATINGOBJECT_H
#define UPDATINGOBJECT_H


class UpdatingObject
{
	public:
		UpdatingObject();
		virtual ~UpdatingObject();
		virtual void Update(float deltaTime) = 0;
	protected:
	private:
};

#endif // UPDATINGOBJECT_H
