$Id: README.txt,v 1.1.2.5.4.3 2010/09/20 18:22:17 weitzman Exp $

The Migrate module provides a flexible framework for migrating content into Drupal from other
sources (e.g., when converting a web site from another CMS to Drupal). Out-of-the-box, support 
for creating Drupal nodes, fields, comments, url aliases, users+profiles and roles are included. 
Plugins permit migration of other types of content.

Usage
----------------
For now, all we offer is documentation by example. Enable the migrate_example module and browse to 
admin/migrate to see its dashboard. The data for this migration is in migrate_example/beer.inc.
Mimic that file in order to specify your own migrations. All imports/rollbacks/etc. are initiated
by drush commands.

Acknowledgements 
----------------

Much of the Migrate module functionality was sponsored by Cyrve, for its clients GenomeWeb 
(http://www.genomeweb.com), The Economist (http://www.economist.com), and Examiner.com 
(http://www.examiner.com). The original code was based on node_import.

Author
------

Mike Ryan - http://drupal.org/user/4420
