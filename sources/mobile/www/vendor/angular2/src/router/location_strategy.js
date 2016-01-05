'use strict';var lang_1 = require('angular2/src/facade/lang');
var core_1 = require('angular2/core');
/**
 * `LocationStrategy` is responsible for representing and reading route state
 * from the browser's URL. Angular provides two strategies:
 * {@link HashLocationStrategy} (default) and {@link PathLocationStrategy}.
 *
 * This is used under the hood of the {@link Location} service.
 *
 * Applications should use the {@link Router} or {@link Location} services to
 * interact with application route state.
 *
 * For instance, {@link HashLocationStrategy} produces URLs like
 * `http://example.com#/foo`, and {@link PathLocationStrategy} produces
 * `http://example.com/foo` as an equivalent URL.
 *
 * See these two classes for more.
 */
var LocationStrategy = (function () {
    function LocationStrategy() {
    }
    return LocationStrategy;
})();
exports.LocationStrategy = LocationStrategy;
/**
 * The `APP_BASE_HREF` token represents the base href to be used with the
 * {@link PathLocationStrategy}.
 *
 * If you're using {@link PathLocationStrategy}, you must provide a provider to a string
 * representing the URL prefix that should be preserved when generating and recognizing
 * URLs.
 *
 * ### Example
 *
 * ```
 * import {Component} from 'angular2/core';
 * import {ROUTER_DIRECTIVES, ROUTER_PROVIDERS, RouteConfig} from 'angular2/router';
 *
 * @Component({directives: [ROUTER_DIRECTIVES]})
 * @RouteConfig([
 *  {...},
 * ])
 * class AppCmp {
 *   // ...
 * }
 *
 * bootstrap(AppCmp, [
 *   ROUTER_PROVIDERS,
 *   PathLocationStrategy,
 *   provide(APP_BASE_HREF, {useValue: '/my/app'})
 * ]);
 * ```
 */
exports.APP_BASE_HREF = lang_1.CONST_EXPR(new core_1.OpaqueToken('appBaseHref'));
function normalizeQueryParams(params) {
    return (params.length > 0 && params.substring(0, 1) != '?') ? ('?' + params) : params;
}
exports.normalizeQueryParams = normalizeQueryParams;
function joinWithSlash(start, end) {
    if (start.length == 0) {
        return end;
    }
    if (end.length == 0) {
        return start;
    }
    var slashes = 0;
    if (start.endsWith('/')) {
        slashes++;
    }
    if (end.startsWith('/')) {
        slashes++;
    }
    if (slashes == 2) {
        return start + end.substring(1);
    }
    if (slashes == 1) {
        return start + end;
    }
    return start + '/' + end;
}
exports.joinWithSlash = joinWithSlash;
//# sourceMappingURL=data:application/json;base64,eyJ2ZXJzaW9uIjozLCJmaWxlIjoibG9jYXRpb25fc3RyYXRlZ3kuanMiLCJzb3VyY2VSb290IjoiIiwic291cmNlcyI6WyJhbmd1bGFyMi9zcmMvcm91dGVyL2xvY2F0aW9uX3N0cmF0ZWd5LnRzIl0sIm5hbWVzIjpbIkxvY2F0aW9uU3RyYXRlZ3kiLCJMb2NhdGlvblN0cmF0ZWd5LmNvbnN0cnVjdG9yIiwibm9ybWFsaXplUXVlcnlQYXJhbXMiLCJqb2luV2l0aFNsYXNoIl0sIm1hcHBpbmdzIjoiQUFBQSxxQkFBeUIsMEJBQTBCLENBQUMsQ0FBQTtBQUNwRCxxQkFBMEIsZUFBZSxDQUFDLENBQUE7QUFFMUM7Ozs7Ozs7Ozs7Ozs7OztHQWVHO0FBQ0g7SUFBQUE7SUFTQUMsQ0FBQ0E7SUFBREQsdUJBQUNBO0FBQURBLENBQUNBLEFBVEQsSUFTQztBQVRxQix3QkFBZ0IsbUJBU3JDLENBQUE7QUFHRDs7Ozs7Ozs7Ozs7Ozs7Ozs7Ozs7Ozs7Ozs7OztHQTRCRztBQUNVLHFCQUFhLEdBQWdCLGlCQUFVLENBQUMsSUFBSSxrQkFBVyxDQUFDLGFBQWEsQ0FBQyxDQUFDLENBQUM7QUFFckYsOEJBQXFDLE1BQWM7SUFDakRFLE1BQU1BLENBQUNBLENBQUNBLE1BQU1BLENBQUNBLE1BQU1BLEdBQUdBLENBQUNBLElBQUlBLE1BQU1BLENBQUNBLFNBQVNBLENBQUNBLENBQUNBLEVBQUVBLENBQUNBLENBQUNBLElBQUlBLEdBQUdBLENBQUNBLEdBQUdBLENBQUNBLEdBQUdBLEdBQUdBLE1BQU1BLENBQUNBLEdBQUdBLE1BQU1BLENBQUNBO0FBQ3hGQSxDQUFDQTtBQUZlLDRCQUFvQix1QkFFbkMsQ0FBQTtBQUVELHVCQUE4QixLQUFhLEVBQUUsR0FBVztJQUN0REMsRUFBRUEsQ0FBQ0EsQ0FBQ0EsS0FBS0EsQ0FBQ0EsTUFBTUEsSUFBSUEsQ0FBQ0EsQ0FBQ0EsQ0FBQ0EsQ0FBQ0E7UUFDdEJBLE1BQU1BLENBQUNBLEdBQUdBLENBQUNBO0lBQ2JBLENBQUNBO0lBQ0RBLEVBQUVBLENBQUNBLENBQUNBLEdBQUdBLENBQUNBLE1BQU1BLElBQUlBLENBQUNBLENBQUNBLENBQUNBLENBQUNBO1FBQ3BCQSxNQUFNQSxDQUFDQSxLQUFLQSxDQUFDQTtJQUNmQSxDQUFDQTtJQUNEQSxJQUFJQSxPQUFPQSxHQUFHQSxDQUFDQSxDQUFDQTtJQUNoQkEsRUFBRUEsQ0FBQ0EsQ0FBQ0EsS0FBS0EsQ0FBQ0EsUUFBUUEsQ0FBQ0EsR0FBR0EsQ0FBQ0EsQ0FBQ0EsQ0FBQ0EsQ0FBQ0E7UUFDeEJBLE9BQU9BLEVBQUVBLENBQUNBO0lBQ1pBLENBQUNBO0lBQ0RBLEVBQUVBLENBQUNBLENBQUNBLEdBQUdBLENBQUNBLFVBQVVBLENBQUNBLEdBQUdBLENBQUNBLENBQUNBLENBQUNBLENBQUNBO1FBQ3hCQSxPQUFPQSxFQUFFQSxDQUFDQTtJQUNaQSxDQUFDQTtJQUNEQSxFQUFFQSxDQUFDQSxDQUFDQSxPQUFPQSxJQUFJQSxDQUFDQSxDQUFDQSxDQUFDQSxDQUFDQTtRQUNqQkEsTUFBTUEsQ0FBQ0EsS0FBS0EsR0FBR0EsR0FBR0EsQ0FBQ0EsU0FBU0EsQ0FBQ0EsQ0FBQ0EsQ0FBQ0EsQ0FBQ0E7SUFDbENBLENBQUNBO0lBQ0RBLEVBQUVBLENBQUNBLENBQUNBLE9BQU9BLElBQUlBLENBQUNBLENBQUNBLENBQUNBLENBQUNBO1FBQ2pCQSxNQUFNQSxDQUFDQSxLQUFLQSxHQUFHQSxHQUFHQSxDQUFDQTtJQUNyQkEsQ0FBQ0E7SUFDREEsTUFBTUEsQ0FBQ0EsS0FBS0EsR0FBR0EsR0FBR0EsR0FBR0EsR0FBR0EsQ0FBQ0E7QUFDM0JBLENBQUNBO0FBckJlLHFCQUFhLGdCQXFCNUIsQ0FBQSIsInNvdXJjZXNDb250ZW50IjpbImltcG9ydCB7Q09OU1RfRVhQUn0gZnJvbSAnYW5ndWxhcjIvc3JjL2ZhY2FkZS9sYW5nJztcbmltcG9ydCB7T3BhcXVlVG9rZW59IGZyb20gJ2FuZ3VsYXIyL2NvcmUnO1xuXG4vKipcbiAqIGBMb2NhdGlvblN0cmF0ZWd5YCBpcyByZXNwb25zaWJsZSBmb3IgcmVwcmVzZW50aW5nIGFuZCByZWFkaW5nIHJvdXRlIHN0YXRlXG4gKiBmcm9tIHRoZSBicm93c2VyJ3MgVVJMLiBBbmd1bGFyIHByb3ZpZGVzIHR3byBzdHJhdGVnaWVzOlxuICoge0BsaW5rIEhhc2hMb2NhdGlvblN0cmF0ZWd5fSAoZGVmYXVsdCkgYW5kIHtAbGluayBQYXRoTG9jYXRpb25TdHJhdGVneX0uXG4gKlxuICogVGhpcyBpcyB1c2VkIHVuZGVyIHRoZSBob29kIG9mIHRoZSB7QGxpbmsgTG9jYXRpb259IHNlcnZpY2UuXG4gKlxuICogQXBwbGljYXRpb25zIHNob3VsZCB1c2UgdGhlIHtAbGluayBSb3V0ZXJ9IG9yIHtAbGluayBMb2NhdGlvbn0gc2VydmljZXMgdG9cbiAqIGludGVyYWN0IHdpdGggYXBwbGljYXRpb24gcm91dGUgc3RhdGUuXG4gKlxuICogRm9yIGluc3RhbmNlLCB7QGxpbmsgSGFzaExvY2F0aW9uU3RyYXRlZ3l9IHByb2R1Y2VzIFVSTHMgbGlrZVxuICogYGh0dHA6Ly9leGFtcGxlLmNvbSMvZm9vYCwgYW5kIHtAbGluayBQYXRoTG9jYXRpb25TdHJhdGVneX0gcHJvZHVjZXNcbiAqIGBodHRwOi8vZXhhbXBsZS5jb20vZm9vYCBhcyBhbiBlcXVpdmFsZW50IFVSTC5cbiAqXG4gKiBTZWUgdGhlc2UgdHdvIGNsYXNzZXMgZm9yIG1vcmUuXG4gKi9cbmV4cG9ydCBhYnN0cmFjdCBjbGFzcyBMb2NhdGlvblN0cmF0ZWd5IHtcbiAgYWJzdHJhY3QgcGF0aCgpOiBzdHJpbmc7XG4gIGFic3RyYWN0IHByZXBhcmVFeHRlcm5hbFVybChpbnRlcm5hbDogc3RyaW5nKTogc3RyaW5nO1xuICBhYnN0cmFjdCBwdXNoU3RhdGUoc3RhdGU6IGFueSwgdGl0bGU6IHN0cmluZywgdXJsOiBzdHJpbmcsIHF1ZXJ5UGFyYW1zOiBzdHJpbmcpOiB2b2lkO1xuICBhYnN0cmFjdCByZXBsYWNlU3RhdGUoc3RhdGU6IGFueSwgdGl0bGU6IHN0cmluZywgdXJsOiBzdHJpbmcsIHF1ZXJ5UGFyYW1zOiBzdHJpbmcpOiB2b2lkO1xuICBhYnN0cmFjdCBmb3J3YXJkKCk6IHZvaWQ7XG4gIGFic3RyYWN0IGJhY2soKTogdm9pZDtcbiAgYWJzdHJhY3Qgb25Qb3BTdGF0ZShmbjogKF86IGFueSkgPT4gYW55KTogdm9pZDtcbiAgYWJzdHJhY3QgZ2V0QmFzZUhyZWYoKTogc3RyaW5nO1xufVxuXG5cbi8qKlxuICogVGhlIGBBUFBfQkFTRV9IUkVGYCB0b2tlbiByZXByZXNlbnRzIHRoZSBiYXNlIGhyZWYgdG8gYmUgdXNlZCB3aXRoIHRoZVxuICoge0BsaW5rIFBhdGhMb2NhdGlvblN0cmF0ZWd5fS5cbiAqXG4gKiBJZiB5b3UncmUgdXNpbmcge0BsaW5rIFBhdGhMb2NhdGlvblN0cmF0ZWd5fSwgeW91IG11c3QgcHJvdmlkZSBhIHByb3ZpZGVyIHRvIGEgc3RyaW5nXG4gKiByZXByZXNlbnRpbmcgdGhlIFVSTCBwcmVmaXggdGhhdCBzaG91bGQgYmUgcHJlc2VydmVkIHdoZW4gZ2VuZXJhdGluZyBhbmQgcmVjb2duaXppbmdcbiAqIFVSTHMuXG4gKlxuICogIyMjIEV4YW1wbGVcbiAqXG4gKiBgYGBcbiAqIGltcG9ydCB7Q29tcG9uZW50fSBmcm9tICdhbmd1bGFyMi9jb3JlJztcbiAqIGltcG9ydCB7Uk9VVEVSX0RJUkVDVElWRVMsIFJPVVRFUl9QUk9WSURFUlMsIFJvdXRlQ29uZmlnfSBmcm9tICdhbmd1bGFyMi9yb3V0ZXInO1xuICpcbiAqIEBDb21wb25lbnQoe2RpcmVjdGl2ZXM6IFtST1VURVJfRElSRUNUSVZFU119KVxuICogQFJvdXRlQ29uZmlnKFtcbiAqICB7Li4ufSxcbiAqIF0pXG4gKiBjbGFzcyBBcHBDbXAge1xuICogICAvLyAuLi5cbiAqIH1cbiAqXG4gKiBib290c3RyYXAoQXBwQ21wLCBbXG4gKiAgIFJPVVRFUl9QUk9WSURFUlMsXG4gKiAgIFBhdGhMb2NhdGlvblN0cmF0ZWd5LFxuICogICBwcm92aWRlKEFQUF9CQVNFX0hSRUYsIHt1c2VWYWx1ZTogJy9teS9hcHAnfSlcbiAqIF0pO1xuICogYGBgXG4gKi9cbmV4cG9ydCBjb25zdCBBUFBfQkFTRV9IUkVGOiBPcGFxdWVUb2tlbiA9IENPTlNUX0VYUFIobmV3IE9wYXF1ZVRva2VuKCdhcHBCYXNlSHJlZicpKTtcblxuZXhwb3J0IGZ1bmN0aW9uIG5vcm1hbGl6ZVF1ZXJ5UGFyYW1zKHBhcmFtczogc3RyaW5nKTogc3RyaW5nIHtcbiAgcmV0dXJuIChwYXJhbXMubGVuZ3RoID4gMCAmJiBwYXJhbXMuc3Vic3RyaW5nKDAsIDEpICE9ICc/JykgPyAoJz8nICsgcGFyYW1zKSA6IHBhcmFtcztcbn1cblxuZXhwb3J0IGZ1bmN0aW9uIGpvaW5XaXRoU2xhc2goc3RhcnQ6IHN0cmluZywgZW5kOiBzdHJpbmcpOiBzdHJpbmcge1xuICBpZiAoc3RhcnQubGVuZ3RoID09IDApIHtcbiAgICByZXR1cm4gZW5kO1xuICB9XG4gIGlmIChlbmQubGVuZ3RoID09IDApIHtcbiAgICByZXR1cm4gc3RhcnQ7XG4gIH1cbiAgdmFyIHNsYXNoZXMgPSAwO1xuICBpZiAoc3RhcnQuZW5kc1dpdGgoJy8nKSkge1xuICAgIHNsYXNoZXMrKztcbiAgfVxuICBpZiAoZW5kLnN0YXJ0c1dpdGgoJy8nKSkge1xuICAgIHNsYXNoZXMrKztcbiAgfVxuICBpZiAoc2xhc2hlcyA9PSAyKSB7XG4gICAgcmV0dXJuIHN0YXJ0ICsgZW5kLnN1YnN0cmluZygxKTtcbiAgfVxuICBpZiAoc2xhc2hlcyA9PSAxKSB7XG4gICAgcmV0dXJuIHN0YXJ0ICsgZW5kO1xuICB9XG4gIHJldHVybiBzdGFydCArICcvJyArIGVuZDtcbn1cbiJdfQ==