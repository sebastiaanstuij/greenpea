module.exports = {
  /**
   * Application configuration section
   * http://pm2.keymetrics.io/docs/usage/application-declaration/
   */
  apps : [

    // First application
    {
      name      : "WEB-API",
      script    : "webserver-rest/app.js",
      env: {
        COMMON_VARIABLE: "true"
      },
      env_production : {
        NODE_ENV: "production"
      }
    },

    // Second application
    {
      name      : "WEB-IOT",
      script    : "webserver-iot/app.js"
    }
  ],

  /**
   * Deployment section
   * http://pm2.keymetrics.io/docs/usage/deployment/
   */
  deploy : {
    production : {
      user : "greenpea",
      host : "192.168.1.104",
      ref  : "origin/master",
      repo : "git@github.com:sebastiaanstuij/greenpea.git",
      path : "/var/www/production",
      "post-deploy" : "npm install && pm2 startOrRestart ecosystem.json --env production"
    },
    dev : {
      user : "greenpea",
      host : "192.168.1.104",
      ref  : "origin/master",
      repo : "git@github.com:sebastiaanstuij/greenpea.git",
      path : "/var/www/development",
      "post-deploy" : "npm install && pm2 startOrRestart ecosystem.json --env dev",
      env  : {
        NODE_ENV: "dev"
      }
    }
  }
}
