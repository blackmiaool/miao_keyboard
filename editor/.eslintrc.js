// https://eslint.org/docs/user-guide/configuring

module.exports = {
    root: true,
    parserOptions: {
        parser: "babel-eslint"
    },
    env: {
        browser: true
    },
    // https://github.com/vuejs/eslint-plugin-vue#priority-a-essential-error-prevent
    // ion consider switching to `plugin:vue/strongly-recommended` or
    // `plugin:vue/recommended` for stricter rules.
    extends: ["plugin:vue/essential", "airbnb-base"],
    // required to lint *.vue files
    plugins: ["vue"],
    // check if imports actually resolve
    settings: {
        "import/resolver": {
            webpack: {
                config: "build/webpack.base.conf.js"
            }
        }
    },
    // add your custom rules here
    rules: {
        // don't require .vue extension when importing
        "import/extensions": [
            "error",
            "always",
            {
                js: "never",
                vue: "never"
            }
        ],
        // disallow reassignment of function parameters disallow parameter object
        // manipulation except for specific exclusions
        "no-param-reassign": [
            "error",
            {
                props: true,
                ignorePropertyModificationsFor: [
                    "state", // for vuex state
                    "acc", // for reduce accumulators
                    "e" // for e.returnvalue
                ]
            }
        ],
        // allow optionalDependencies
        "import/no-extraneous-dependencies": [
            "error",
            {
                optionalDependencies: ["test/unit/index.js"]
            }
        ],
        // allow debugger during development
        "no-debugger": process.env.NODE_ENV === "production" ? "error" : "off",
        "no-console": 0,
        "arrow-body-style": 0,
        "arrow-parens": 0,
        quotes: 0,
        "comma-dangle": 0,
        "max-len": 0,
        indent: 0,
        "no-loop-func": 0,
        "no-param-reassign": 0,
        "no-use-before-define": [
            "error",
            {
                functions: false
            }
        ],
        "linebreak-style": 0,
        "no-plusplus": 0,
        "no-alert": 0,
        "no-new": 0
    }
};
