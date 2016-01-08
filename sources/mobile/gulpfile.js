var gulp = require('gulp'),
    sass = require('gulp-sass'),
    notify = require("gulp-notify"),
    bower = require('gulp-bower'),
    ts = require('gulp-typescript'),
    run = require('gulp-run'),
    debug = require('gulp-debug');;
    
var config = {
    baseDir: './www',
    sassPath: './www/sass',
    bowerDir: './www/bower_components'
}

var tsProject = ts.createProject('./tsconfig.json');

var sassOptions = {
  errLogToConsole: true,
  outputStyle: 'expanded'
};


// Run gulp bower to install bower components (equivalent to : bower install) 
gulp.task('bower', function() {
    return bower()
        .pipe(gulp.dest(config.bowerDir))
});

gulp.task('icons', function() {
    return gulp.src(config.bowerDir + '/fontawesome/fonts/**.*')
        .pipe(gulp.dest(config.baseDir + '/fonts'));
});

gulp.task('css', function() {
    gulp.src(config.sassPath + '/*.scss')
        .pipe(sass.sync(sassOptions))
        .pipe(gulp.dest(config.baseDir + '/css'));
    /*return sass(config.sassPath + '/style.scss', {
            style: 'compressed',
            loadPath: [
                config.sassPath,
                config.bowerDir + '/bootstrap-sass-official/assets/stylesheets',
                config.bowerDir + '/fontawesome/scss',
            ]
        })
        .pipe(gulp.dest(config.baseDir + '/css'));*/
});

gulp.task('typescript', function() {
	return gulp.src(config.baseDir + '/app/**/*.ts')
        .pipe(debug({title: 'ts files:'}))
		.pipe(ts(tsProject))
        .pipe(gulp.dest(config.baseDir + '/app'));
});

// Rerun the task when a file changes
gulp.task('watch', function() {
    gulp.watch(config.sassPath + '/**/*.scss', ['css']);
    gulp.watch(config.baseDir + '/app/**/*.ts', ['typescript']);
});

gulp.task('build', ['bower','typescript', 'icons', 'css']);

gulp.task('server', function ()
{
    run('echo Running lite-server...').exec();
    run('lite-server --baseDir ./www').exec();
});

gulp.task('run', ['build', 'watch', 'server']);