// BrainClass.h
// Reconstructed from GAME.EXE (C&C Tiberian Sun, Westwood Studios 1999).
// Evidence: RTTI COL 0x6aa298 -> ".?AVBrainClass@@".
// vtable VA 0x69a260, 1 slot (0x00) = scalar deleting destructor 0x424dc0.
//
// Object layout (destructor 0x424dc0):
//   +0x00  vftable (0x69a260)
//   +0x04  VectorClass<NeuronClass*> m_Neurons /* ? */ (vtable 0x69a244)
//   +0x08  NeuronClass** m_pNeuronArray /* ? */ (freed element-wise in dtor)
//   +0x14  int m_nNeuronCount /* ? */
//
// BrainClass owns a NeuronClass network (RTTI ".?AVNeuronClass@@").  The
// destructor walks m_pNeuronArray[0..m_nNeuronCount) and releases each neuron
// via its vtable slot 0x20, then destroys the embedded VectorClass<NeuronClass*>.
//
// No INI keys are read directly by BrainClass; the AI brain parameters
// (AITriggerSuccessWeightDelta, AITriggerFailureWeightDelta,
// AITriggerTrackRecordCoefficient) belong to AITriggerTypeClass (see
// TriggerTypeClass.h).

#ifndef BRAINCLASS_H
#define BRAINCLASS_H

#include <windows.h>

class NeuronClass;

class BrainClass
{
public:
    // +0x00  vftable (0x69a260)
    // +0x04  VectorClass<NeuronClass*> m_Neurons /* ? */
    // +0x08  NeuronClass** m_pNeuronArray /* ? */
    // +0x14  int m_nNeuronCount /* ? */

    virtual ~BrainClass();                   // 0x424dc0 (scalar deleting dtor)

    // ---- data members (offsets from ctor 0x00424925) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69a260

};

#endif // BRAINCLASS_H
