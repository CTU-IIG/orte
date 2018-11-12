qt4 = import('qt4')

qt_dep = dependency('qt4', modules: ['Core', 'Gui'])

if qt_dep.found()

  preprocessed = qt4.preprocess(
    moc_headers: [
    'FPublisher.h',
    'FSubscriber.h',
    'MainForm.h',
    'MyQFrame.h',
    'richtext.h',
  ],
    ui_files: [ 'MainForm.ui', 'FPublisher.ui', 'FSubscriber.ui' ],
    qresources: 'shapes.qrc',
  )

  executable(
    'ortedemo',
    sources: [
      'main.cpp',
      'MainForm.cpp',
      'MyQFrame.cpp',
      'richtext.cpp',
      'FPublisher.cpp',
      'FSubscriber.cpp',
      'BoxType.c',
      preprocessed,
    ],
    include_directories: include_directories('../../include'),
    dependencies: [
      liborte_dep,
      qt_dep,
    ]
  )

endif
