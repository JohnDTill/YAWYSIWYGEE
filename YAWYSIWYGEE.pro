TEMPLATE = subdirs

SUBDIRS = TypesetWidget ExampleApp Tests

ExampleApp.depends = TypesetWidget
Tests.depends = TypesetWidget
