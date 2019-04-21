#include <cstdint>
#include <memory>


class VMTHooking {

	private:
		size_t m_numFuncs = 0;
		uintptr_t* m_vtable = nullptr;
		std::unique_ptr<std::uintptr_t[]> m_copiedVTable = NULL;
		uintptr_t** m_classPointer = nullptr;

		void GetVFuncSize(uintptr_t** classPointer) {
			m_vtable = *classPointer;
			while (reinterpret_cast<uintptr_t*>(classPointer)[m_numFuncs]) {
				m_numFuncs += 1;
			}
		}

	public:
		VMTHooking()
		{

		}
		uintptr_t* GetReplacedVTablePointer(void* classToHook) {
			if (classToHook) {
				m_classPointer = reinterpret_cast<std::uintptr_t**>(classToHook);
				m_vtable = *m_classPointer;
				while (reinterpret_cast<std::uintptr_t*>(m_vtable[m_numFuncs])) {
					m_numFuncs += 1;
				}
				if (m_numFuncs > 0) {
					m_copiedVTable = std::make_unique<std::uintptr_t[]>(m_numFuncs);
					std::memcpy(m_copiedVTable.get(), m_vtable, m_numFuncs * sizeof(std::uintptr_t));
				}
			}
			/*m_classPointer = reinterpret_cast<uintptr_t**>(classToHook);
			GetVFuncSize(m_classPointer);

			m_copiedVTable = std::make_unique<std::uintptr_t[]>(m_numFuncs);
			memcpy(m_copiedVTable.get(), m_vtable, m_numFuncs * sizeof(std::uintptr_t));*/
			return m_copiedVTable.get();
		}

		uintptr_t HookVMT(const int funcIndexToHook, uintptr_t detourTarget) {
			m_copiedVTable[funcIndexToHook] = detourTarget;
			*m_classPointer = m_copiedVTable.get();
			return m_vtable[funcIndexToHook];
		}

		void Restore(const int funcIndexToRestore) {
			m_copiedVTable[funcIndexToRestore] = m_vtable[funcIndexToRestore];
			*m_classPointer = m_copiedVTable.get();
		}
};
