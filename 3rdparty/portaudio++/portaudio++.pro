TARGET = portaudioxx
TEMPLATE = lib
CONFIG += staticlib

QT -= core gui

include ($$SOURCE_TREE/common.pri)

INCLUDEPATH += \
    $$PORTAUDIO_PATH/include \
    include

SOURCES += \
    source/portaudiocpp/SystemHostApiIterator.cxx \
    source/portaudiocpp/SystemDeviceIterator.cxx \
    source/portaudiocpp/System.cxx \
    source/portaudiocpp/StreamParameters.cxx \
    source/portaudiocpp/Stream.cxx \
    source/portaudiocpp/MemFunCallbackStream.cxx \
    source/portaudiocpp/InterfaceCallbackStream.cxx \
    source/portaudiocpp/HostApi.cxx \
    source/portaudiocpp/Exception.cxx \
    source/portaudiocpp/DirectionSpecificStreamParameters.cxx \
    source/portaudiocpp/Device.cxx \
    source/portaudiocpp/CppFunCallbackStream.cxx \
    source/portaudiocpp/CFunCallbackStream.cxx \
    source/portaudiocpp/CallbackStream.cxx \
    source/portaudiocpp/CallbackInterface.cxx \
    source/portaudiocpp/BlockingStream.cxx
    
    #source/portaudiocpp/AsioDeviceAdapter.cxx

HEADERS += \
    include/portaudiocpp/SystemHostApiIterator.hxx \
    include/portaudiocpp/SystemDeviceIterator.hxx \
    include/portaudiocpp/System.hxx \
    include/portaudiocpp/StreamParameters.hxx \
    include/portaudiocpp/Stream.hxx \
    include/portaudiocpp/SampleDataFormat.hxx \
    include/portaudiocpp/PortAudioCpp.hxx \
    include/portaudiocpp/MemFunCallbackStream.hxx \
    include/portaudiocpp/InterfaceCallbackStream.hxx \
    include/portaudiocpp/HostApi.hxx \
    include/portaudiocpp/Exception.hxx \
    include/portaudiocpp/DirectionSpecificStreamParameters.hxx \
    include/portaudiocpp/Device.hxx \
    include/portaudiocpp/CppFunCallbackStream.hxx \
    include/portaudiocpp/CFunCallbackStream.hxx \
    include/portaudiocpp/CallbackStream.hxx \
    include/portaudiocpp/CallbackInterface.hxx \
    include/portaudiocpp/BlockingStream.hxx \
    include/portaudiocpp/AutoSystem.hxx
    
    #include/portaudiocpp/AsioDeviceAdapter.hxx


