#include "StdAfxflash.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3Matrix3D.h"
namespace avmshell{
Matrix3DClass::Matrix3DClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* Matrix3DClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) Matrix3DObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

Matrix3DObject*	Matrix3DClass::AS3_interpolate(Matrix3DObject *pThisMat, Matrix3DObject *pToMat, double percent)
{
	Matrix3DObject*pNew=CreateObject();
	{//
	}
	return pNew;
}

Matrix3DObject* Matrix3DClass::CreateObject()
{
	Matrix3DObject*pNew=(Matrix3DObject*)createInstance(ivtable(),prototype);
	return pNew;
}

Matrix3DObject::Matrix3DObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
	/*DoubleVectorClass *pVectorCls = toplevel()->GetdoubleVectorClass();
	m_pRawData = pVectorCls->newVector(16);		//内存管理模块自动将内存初始化为0

	m_pRawData->_setNativeUintProperty(0, 1);
//	m_pRawData[1] = m_pRawData[2] = m_pRawData[3] = 0;

	m_pRawData->_setNativeUintProperty(5, 1);
//	m_pRawData[4] = m_pRawData[6] = m_pRawData[7] = 0;

	m_pRawData->_setNativeUintProperty(10, 1);
//	m_pRawData[8] = m_pRawData[9] = m_pRawData[11] = 0;

	m_pRawData->_setNativeUintProperty(15, 1);
//	m_pRawData[12] = m_pRawData[13] = m_pRawData[14] = 0;



//	m_pPosition = (Vector3DObject*) (&m_pRawData[12]);
	m_pPosition = (Vector3DObject*)(&m_pRawData[12]);*/

	m_dData[0]=1;
	m_dData[1]=0;
	m_dData[2]=0;
	m_dData[3]=0;

	m_dData[4]=0;
	m_dData[5]=1;
	m_dData[6]=0;
	m_dData[7]=0;

	m_dData[8]=0;
	m_dData[9]=0;
	m_dData[10]=1;
	m_dData[11]=0;

	m_dData[12]=0;
	m_dData[13]=0;
	m_dData[14]=0;
	m_dData[15]=1;
	m_determinant = -1;
}
//////////////////////////////////////////////////////////
//Native Method start...
double	Matrix3DObject::AS3_determinant_get()
{
	return m_determinant;
}

Vector3DObject*	Matrix3DObject::AS3_position_get()
{
	ShellToplevel*top=(ShellToplevel*)toplevel();
	Vector3DClass*pClass=top->getVector3DClass();
	Vector3DObject*pObj=pClass->CreateObject(m_dData[12],m_dData[13],m_dData[14]);
	//pObj->SetX(m_dData[12]);
	//pObj->Set(m_dData[12]);
	//pObj->SetX(m_dData[12]);
	return pObj;
	//return m_pPosition;
}
void	Matrix3DObject::AS3_position_set(Vector3DObject *pPosition)
{
	//m_pPosition = pPosition;
	if(pPosition)
	{
		m_dData[12]=pPosition->GetX();
		m_dData[13]=pPosition->GetY();
		m_dData[14]=pPosition->GetZ();
	}
	//update m_pRowData
}

DoubleVectorObject*	Matrix3DObject::AS3_rawData_get()
{
	ShellToplevel*top=(ShellToplevel*)toplevel();
	DoubleVectorClass*pClass=top->doubleVectorClass;
	DoubleVectorObject*pObj=pClass->newVector(16);
	int i;
	for(i=0;i<16;i++)
		pObj->_setNativeIntProperty(i,m_dData[i]);
	return pObj;
	
	//return m_pRawData;
}
void	Matrix3DObject::AS3_rawData_set(DoubleVectorObject *pRawData)
{
	if(pRawData)
	{
		int i;
		for(i=0;i<16;i++)
		{
			m_dData[i]=pRawData->_getNativeIntProperty(i);
		}
	}
	//m_pRawData = pRawData;
	//update m_pPosition
}

void	Matrix3DObject::AS3_constructor(DoubleVectorObject *pV)
{
	if(pV)
	{
		int i;
		for(i=0;i<16;i++)
		{
			m_dData[i]=pV->_getNativeIntProperty(i);
		}
	}
}

void Matrix3DObject::PrependData(double dData[16])
{
		double tmp[16];
		tmp[0]=m_dData[0]*dData[0]+m_dData[1]*dData[4]+m_dData[2]*dData[8]+m_dData[3]*dData[12];
		tmp[1]=m_dData[0]*dData[1]+m_dData[1]*dData[5]+m_dData[2]*dData[9]+m_dData[3]*dData[13];
		tmp[2]=m_dData[0]*dData[2]+m_dData[1]*dData[6]+m_dData[2]*dData[10]+m_dData[3]*dData[14];
		tmp[3]=m_dData[0]*dData[3]+m_dData[1]*dData[7]+m_dData[2]*dData[11]+m_dData[3]*dData[15];

		tmp[4]=m_dData[4]*dData[0]+m_dData[5]*dData[4]+m_dData[6]*dData[8]+m_dData[7]*dData[12];
		tmp[5]=m_dData[4]*dData[1]+m_dData[5]*dData[5]+m_dData[6]*dData[9]+m_dData[7]*dData[13];
		tmp[6]=m_dData[4]*dData[2]+m_dData[5]*dData[6]+m_dData[6]*dData[10]+m_dData[7]*dData[14];
		tmp[7]=m_dData[4]*dData[3]+m_dData[5]*dData[7]+m_dData[6]*dData[11]+m_dData[7]*dData[15];

		tmp[8]=m_dData[8]*dData[0]+m_dData[9]*dData[4]+m_dData[10]*dData[8]+m_dData[11]*dData[12];
		tmp[9]=m_dData[8]*dData[1]+m_dData[9]*dData[5]+m_dData[10]*dData[9]+m_dData[11]*dData[13];
		tmp[10]=m_dData[8]*dData[2]+m_dData[9]*dData[6]+m_dData[10]*dData[10]+m_dData[11]*dData[14];
		tmp[11]=m_dData[8]*dData[3]+m_dData[9]*dData[7]+m_dData[10]*dData[11]+m_dData[11]*dData[15];

		tmp[12]=m_dData[12]*dData[0]+m_dData[13]*dData[4]+m_dData[14]*dData[8]+m_dData[15]*dData[12];
		tmp[13]=m_dData[12]*dData[1]+m_dData[13]*dData[5]+m_dData[14]*dData[9]+m_dData[15]*dData[13];
		tmp[14]=m_dData[12]*dData[2]+m_dData[13]*dData[6]+m_dData[14]*dData[10]+m_dData[15]*dData[14];
		tmp[15]=m_dData[12]*dData[3]+m_dData[13]*dData[7]+m_dData[14]*dData[11]+m_dData[15]*dData[15];

		memcpy(m_dData,tmp,sizeof(tmp));
}


void Matrix3DObject::AppendData(double dData[16])
{
		double tmp[16];
		tmp[0]=dData[0]*m_dData[0]+dData[1]*m_dData[4]+dData[2]*m_dData[8]+dData[3]*m_dData[12];
		tmp[1]=dData[0]*m_dData[1]+dData[1]*m_dData[5]+dData[2]*m_dData[9]+dData[3]*m_dData[13];
		tmp[2]=dData[0]*dData[2]+m_dData[1]*m_dData[6]+dData[2]*m_dData[10]+m_dData[3]*m_dData[14];
		tmp[3]=dData[0]*dData[3]+m_dData[1]*m_dData[7]+dData[2]*m_dData[11]+m_dData[3]*m_dData[15];

		tmp[4]=dData[4]*m_dData[0]+dData[5]*m_dData[4]+dData[6]*m_dData[8]+dData[7]*m_dData[12];
		tmp[5]=dData[4]*m_dData[1]+dData[5]*m_dData[5]+dData[6]*m_dData[9]+dData[7]*m_dData[13];
		tmp[6]=dData[4]*m_dData[2]+dData[5]*m_dData[6]+dData[6]*m_dData[10]+dData[7]*m_dData[14];
		tmp[7]=dData[4]*m_dData[3]+dData[5]*m_dData[7]+dData[6]*m_dData[11]+dData[7]*m_dData[15];

		tmp[8]=dData[8]*m_dData[0]+dData[9]*m_dData[4]+dData[10]*m_dData[8]+dData[11]*m_dData[12];
		tmp[9]=dData[8]*m_dData[1]+dData[9]*m_dData[5]+dData[10]*m_dData[9]+dData[11]*m_dData[13];
		tmp[10]=dData[8]*m_dData[2]+dData[9]*m_dData[6]+dData[10]*m_dData[10]+dData[11]*m_dData[14];
		tmp[11]=dData[8]*m_dData[3]+dData[9]*m_dData[7]+dData[10]*m_dData[11]+dData[11]*m_dData[15];

		tmp[12]=dData[12]*m_dData[0]+dData[13]*m_dData[4]+dData[14]*m_dData[8]+dData[15]*m_dData[12];
		tmp[13]=dData[12]*m_dData[1]+dData[13]*m_dData[5]+dData[14]*m_dData[9]+dData[15]*m_dData[13];
		tmp[14]=dData[12]*m_dData[2]+dData[13]*m_dData[6]+dData[14]*m_dData[10]+dData[15]*m_dData[14];
		tmp[15]=dData[12]*m_dData[3]+dData[13]*m_dData[7]+dData[14]*m_dData[11]+dData[15]*m_dData[15];

		memcpy(m_dData,tmp,sizeof(tmp));
}

void	Matrix3DObject::AS3_append(Matrix3DObject *pLhs)
{
	if(pLhs)
	{
		AppendData(pLhs->m_dData);

	}
}
void	Matrix3DObject::AS3_appendRotation(double degrees, Vector3DObject *pAxis, Vector3DObject *pPivotPoint)
{

}
void	Matrix3DObject::AS3_appendScale(double xScale, double yScale, double zScale)
{
	double tmp[16]=
	{xScale,0,0,0,
	 0,yScale,0,0,
	 0,0,zScale,0,
	 0,0,0,1};
	AppendData(tmp);
}
void	Matrix3DObject::AS3_appendTranslation(double x, double y, double z)
{
	//m_dData[12]+=x;
	//m_dData[13]+=y;
	//m_dData[14]+=x;
	double tmp[16]=
	{1,0,0,0,
	 0,1,0,0,
	 0,0,1,0,
	 x,y,z,1};
	AppendData(tmp);
}

Matrix3DObject*	Matrix3DObject::AS3_clone()
{
	Matrix3DClass*pClass=((ShellToplevel*)toplevel())->getMatrix3DClass();
	Matrix3DObject*pObj=pClass->CreateObject();
	memcpy(pObj->m_dData,m_dData,sizeof(m_dData));
	return pObj;
}
ObjectVectorObject*	Matrix3DObject::AS3_decompose(Stringp orientationStyle)
{
	//ShellToplevel*top=(ShellToplevel*)toplevel();
	//Vector3DClass*pClass=top->getVector3DClass();
	return NULL;
}
Vector3DObject*	Matrix3DObject::AS3_deltaTransformVector(Vector3DObject *pV)
{
	ShellToplevel*top=(ShellToplevel*)toplevel();
	Vector3DClass*pClass=top->getVector3DClass();
	Vector3DObject*pObj=pClass->CreateObject(0,0,0);
	if(pV)
	{
		ShellToplevel*top=(ShellToplevel*)toplevel();
		
		double x=pV->GetX()*m_dData[0]+pV->GetY()*m_dData[4]+pV->GetZ()*m_dData[8];
		double y=pV->GetX()*m_dData[1]+pV->GetY()*m_dData[5]+pV->GetZ()*m_dData[9];
		double z=pV->GetX()*m_dData[2]+pV->GetY()*m_dData[6]+pV->GetZ()*m_dData[10];
		pObj->SetX(x);
		pObj->SetY(y);
		pObj->SetZ(z);
	}
	return pObj;
	//return NULL;
}
void	Matrix3DObject::AS3_identity()
{
	m_dData[0]=1;
	m_dData[1]=0;
	m_dData[2]=0;
	m_dData[3]=0;

	m_dData[4]=0;
	m_dData[5]=1;
	m_dData[6]=0;
	m_dData[7]=0;

	m_dData[8]=0;
	m_dData[9]=0;
	m_dData[10]=1;
	m_dData[11]=0;

	m_dData[12]=0;
	m_dData[13]=0;
	m_dData[14]=0;
	m_dData[15]=1;
	m_determinant = -1;
}

void	Matrix3DObject::AS3_interpolateTo(Matrix3DObject *pToMat, double percent)
{
	if(pToMat)
	{
		if(percent<=0) return;
		else if(percent>=1)
			memcpy(m_dData,pToMat->m_dData,sizeof(m_dData));
		else
		{
			int i;
			double f=1-percent;
			for(i=0;i<16;i++)
			{
				m_dData[i]=m_dData[i]*f+pToMat->m_dData[i]*percent;
			}
		}
	}
}
bool	Matrix3DObject::AS3_invert()
{
	return false;
}
void	Matrix3DObject::AS3_pointAt(Vector3DObject *pPos, Vector3DObject *pAt, Vector3DObject *pUp)
{

}
void	Matrix3DObject::AS3_prepend(Matrix3DObject *pRhs)
{
	if(pRhs)
		PrependData(pRhs->m_dData);
}
void	Matrix3DObject::AS3_prependRotation(double degrees, Vector3DObject *pAxis, Vector3DObject *pPivotPoint)
{

}

void	Matrix3DObject::AS3_prependScale(double xScale, double yScale, double zScale)
{
	double tmp[16]=
	{xScale,0,0,0,
	 0,yScale,0,0,
	 0,0,zScale,0,
	 0,0,0,	    1};
	PrependData(tmp);
}
void	Matrix3DObject::AS3_prependTranslation(double x, double y, double z)
{
	m_dData[12]+=x;
	m_dData[13]+=y;
	m_dData[14]+=x;
}
bool	Matrix3DObject::AS3_recompose(ObjectVectorObject *pComponents, Stringp orientationStyle)
{
	return false;
}
Vector3DObject*	Matrix3DObject::AS3_transformVector(Vector3DObject *pV)
{
	ShellToplevel*top=(ShellToplevel*)toplevel();
	Vector3DClass*pClass=top->getVector3DClass();
	Vector3DObject*pObj=pClass->CreateObject(0,0,0);
	if(pV)
	{
		ShellToplevel*top=(ShellToplevel*)toplevel();
		
		double x=pV->GetX()*m_dData[0]+pV->GetY()*m_dData[4]+pV->GetZ()*m_dData[8]+m_dData[12];
		double y=pV->GetX()*m_dData[1]+pV->GetY()*m_dData[5]+pV->GetZ()*m_dData[9]+m_dData[13];
		double z=pV->GetX()*m_dData[2]+pV->GetY()*m_dData[6]+pV->GetZ()*m_dData[10]+m_dData[14];
		pObj->SetX(x);
		pObj->SetY(y);
		pObj->SetZ(z);
	}
	return pObj;
}
void	Matrix3DObject::AS3_transformVectors(ObjectVectorObject *pVin, ObjectVectorObject *pVout)
{
	ShellToplevel*top=(ShellToplevel*)toplevel();
	Vector3DClass*pClass=top->getVector3DClass();
	if(pVin&&pVout)
	{
		int i,iLen=pVin->get_length();
		for(i=0;i<iLen;i++)
		{
			Atom atom=pVin->getUintProperty(i);
			if(AvmCore::isNullOrUndefined(atom)) continue;
			if(atomKind(atom)!=kObjectType) continue;
			Vector3DObject*pObj=(Vector3DObject*)AvmCore::atomToScriptObject(atom);
			if(!ShellCore::IsTypeClass(pObj,pClass))
				continue;
			pObj=AS3_transformVector(pObj);
			pVout->setUintProperty(i,pObj->atom());
		}
	}
}

//new adds 11
void Matrix3DObject::AS3_copyColumnFrom(uint32_t column, Vector3DObject* vector3D)
{

}
void Matrix3DObject::AS3_copyColumnTo(uint32_t column, Vector3DObject* vector3D)
{
	LOGWHERE();
}
void Matrix3DObject::AS3_copyFrom(Matrix3DObject* sourceMatrix3D)
{
	LOGWHERE();
}
void Matrix3DObject::AS3_copyRawDataFrom(DoubleVectorObject* vector, uint32_t index, bool transpose)
{
	LOGWHERE();
}
void Matrix3DObject::AS3_copyRawDataTo(DoubleVectorObject* vector, uint32_t index, bool transpose)
{
	LOGWHERE();
}
void Matrix3DObject::AS3_copyRowFrom(uint32_t column, Vector3DObject* vector3D)
{
	LOGWHERE();
}
void Matrix3DObject::AS3_copyRowTo(uint32_t column, Vector3DObject* vector3D)
{
	LOGWHERE();
}
void Matrix3DObject::AS3_copyToMatrix3D(Matrix3DObject*dest)
{
	LOGWHERE();
}
//new adds 11 end

}