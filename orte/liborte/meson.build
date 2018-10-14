liborte_args = []

if (host_machine.system() == 'windows')
  liborte_args += ['-include', 'orte/orte_config_omk_win32.h']
elif (host_machine.system() == 'linux')
  liborte_args += ['-include', 'orte/orte_config_omk_linux.h']
else
  liborte_args += ['-include', 'orte/orte_config_omk_linux.h']
endif



liborte = library(
  'orte',
  [
    'conv.c',
    'debug.c',
    'event.c',
    'fnmatch.c',
    'htimerNtp.c',
    'objectEntry.c',
    'objectEntryTimer.c',
    'objectUtils.c',
    'ORTEAppRecvThread.c',
    'ORTEAppSendThread.c',
    'ORTEDomainApp.c',
    'ORTEPublication.c',
    'ORTEPublicationTimer.c',
    'ORTESubscription.c',
    'ORTEDomain.c',
    'ORTEDomainMgr.c',
    'ORTEInit.c',
    'ORTEThreadUtils.c',
    'ORTETypeRegister.c',
    'ORTEVerbosity.c',
    'ORTEPattern.c',
    'ORTEMisc.c',
    'parameter.c',
    'RTPSAck.c',
    'RTPSCSTReader.c',
    'RTPSCSTReaderTimer.c',
    'RTPSCSTReaderProc.c',
    'RTPSCSTWriter.c',
    'RTPSCSTWriterTimer.c',
    'RTPSGap.c',
    'RTPSHeader.c',
    'RTPSHeardBeat.c',
    'RTPSInfoDST.c',
    'RTPSInfoREPLY.c',
    'RTPSInfoSRC.c',
    'RTPSInfoTS.c',
    'RTPSPad.c',
    'RTPSUtils.c',
    'RTPSVar.c',
    'RTPSIssue.c',
    'sock.c',
    'ul_gavl.c',
    'ul_gavlprim.c',
    'cdr.c',
  ],
  dependencies: dependency('threads'),
  include_directories: include_directories('../include'),
  c_args: [ '-DHAVE_CONFIG_H', liborte_args ],
  install: true,
)

liborte_dep = declare_dependency(
  link_with: liborte,
  include_directories: include_directories('../include'),
  compile_args: [ '-DHAVE_CONFIG_H' ] + liborte_args,
)
