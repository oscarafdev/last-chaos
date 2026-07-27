#ifndef __PET_STASH_MANAGER_H__
#define __PET_STASH_MANAGER_H__

class CPetStashManager
{
public:
	explicit CPetStashManager(CPC* pc);
	~CPetStashManager(void);

	bool	Insert(std::vector<CItem*> &vec);						//Carga los datos del deposito de mascotas
	void	AddPetToStash(CItem* petItem);							//Agrega la mascota al deposito
	void	DelPetToStash(CItem* petItem);							//Quita la mascota del deposito
	void	SendPetStashList();									//Lista de mascotas
	void	SelProxyPet(CItem* petItem);							//Configura la mascota delegada
	void	CancelProxyPet(CItem* petItem);							//Cancela la mascota delegada
	void	ExpireTime();							//Cancela la mascota delegada (accion del temporizador)
	void	DelPetItem(CItem* petItem);
	void	GetDataToQuery( std::vector<std::string>& vec );
	CItem*	GetPetItemByVIndex(int index);
	CItem*  GetPetItemByPlus(int plus);
	CAPet*	getProxyAPet();
	CPet*	getProxyPet();
	void	ApplyEffect();
	bool	FindApet(CAPet* apet);
	bool	FindPet(CPet* pet);

	int	getCount()
	{
		return m_count;
	}
	void setCount(int count)
	{
		if (count > MAX_PET_STASH_KEEP_COUNT)
			count = MAX_PET_STASH_KEEP_COUNT;

		m_count = count;
	}
	int getProxyPetItemvIndex()
	{
		return proxyPetItemVIndex;
	}
	void setProxyPetItemvIndex( int index )
	{
		proxyPetItemVIndex = index;
	}
	int getProxyPetItemPlus()
	{
		return proxyPetItemPlus;
	}
	void setProxyPetItemPlus( int plus )
	{
		proxyPetItemPlus = plus;
	}
	void setEffect(int effectNo)
	{
		m_effectNo = effectNo;
	}
	int getEffect()
	{
		return m_effectNo;
	}

	void UpdatePetData(int petIndex);
	std::vector<CItem*> m_petItem;

private:
	int proxyPetItemVIndex;
	int proxyPetItemPlus;
	int m_effectNo;
	short m_count;
	CPC* _owner;
	CItem* keepPetItem;
};

#endif
