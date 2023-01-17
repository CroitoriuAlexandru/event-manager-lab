pipeline {

    agent any
    // enviroment {
    //     // define variables to be added to enviroment
    //     NEW_VERSION = '1.3.0'
    // }
    stages {
        stage('build'){
           steps {
               echo "Building ..."
            // archiveArtifacts artifacts: 'bin/Debug', fingerprint: true
            }
        }
        stage('test') {
            steps {
                echo "Testing ..."
            }
        }
        stage('deploy') {
            steps {
                echo "Deploy ..."
            }
        }
    }
    // post {
    //     always {
    //         // executs always
    //     }
    //     success {
    //         // executs if build success
    //     }
    //     failure {
    //         // executes on build fail
    //     }
    // }
}