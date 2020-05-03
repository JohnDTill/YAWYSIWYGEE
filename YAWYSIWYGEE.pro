TEMPLATE = subdirs

SUBDIRS = source ExampleApp Tests

ExampleApp.depends = source
Tests.depends = source
