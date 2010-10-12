// $Id: migrate_ui.js,v 1.1.4.5 2010/07/14 19:57:38 weitzman Exp $

/**
 * Provide the summary information for the migration detail vertical tabs.
 * TODO: Provide summary for Mapping tabs.
 */
 
 // The following line will prevent a JavaScript error if this file is included and vertical tabs are disabled.
 Drupal.verticalTabs = Drupal.verticalTabs || {};

 // Note the name here matches the name of the fieldset ID.
 Drupal.verticalTabs.destination = function() {
       context = $('fieldset.vertical-tabs-destination');
       total = $('tr', context).length - 2;
       unmapped = $('td.migrate-error', context).length / 2;
       mapped = total - unmapped;
       msg = Drupal.formatPlural(mapped, '1 mapping.', '@count mapped.');
       if (unmapped) {
         msg = '<span class="error">' + Drupal.formatPlural(unmapped, '1 unmapped', '@count unmapped') + '</span>' + '. ' + msg;
       }
       return msg;
 }
 
 Drupal.verticalTabs.source = function() {
      context = $('fieldset.vertical-tabs-source');
      total = $('tr', context).length - 2;
      unmapped = $('td.migrate-error', context).length / 2;
      mapped = total - unmapped;
      msg = Drupal.formatPlural(mapped, '1 mapping.', '@count mapped.');
      if (unmapped) {
        msg = '<span class="error">' + Drupal.formatPlural(unmapped, '1 unmapped', '@count unmapped') + '</span>' + '. ' + msg;
      }
      return msg;
 }