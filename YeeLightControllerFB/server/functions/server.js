/***
* @Project: Firebase Authenticaiton demo using nodejs
* @Author: Wayan Agus Yudiana
* @Description: This Application is providing the login and token validation method using Firebase Authentication. 
* @URL: /login, /validatetoken  
****/

const functions = require('firebase-functions');
const admin = require('firebase-admin');
const firebase = require('firebase');
const bodyParser = require('body-parser');


const cors = require('cors');
const express = require('express');
const cookieParser = require('cookie-parser')();


/** This configuration is created in the firebase console project application */
const firebaseConfig = {
    apiKey: "[Your API Key]",
    authDomain: "[Your Firebase Project Name].firebaseapp.com",
    databaseURL: "https://[Your Firebase Project Name].firebaseio.com",
    projectId: "[Your Firebase Project Name]",
    storageBucket: "[Your Firebase Project Name].appspot.com",
    messagingSenderId: "[Messaging Sender Id]",
    appId: "[APP ID]"
  };


const headerData = {
    'Content-Type': 'text/event-stream',
    'Connection': 'Keep-Alive',
    'Cache-Control': 'no-cache'
    //'Keep-Alive': 'timeout=50, max=1000'
};


admin.initializeApp(firebaseConfig);
firebase.initializeApp(firebaseConfig);

const auth = firebase.auth();

const app = express();
const port = 3000;

app.use(cors());
app.use(bodyParser.urlencoded({ extended: true }));


function getDateNow()
{
    var today = new Date(); 
    var dd = today.getDate(); 
    var mm = today.getMonth() + 1; 
    var hour = today.getHours();
    var min = today.getMinutes();
    var sec = today.getSeconds();
  
    var yyyy = today.getFullYear(); 
    if (dd < 10) { 
        dd = '0' + dd; 
    } 
    if (mm < 10) { 
        mm = '0' + mm; 
    } 
    today = dd + '/' + mm + '/' + yyyy + ' ' + hour +':'+ min +':'+sec; 
    return today;
}

function printLog(msg,TAG="")
{
    if(TAG==="")
        console.log(`[${getDateNow()}]${msg}`);
    else
        console.log(`[${getDateNow()}][${TAG}]${msg}`);
}



/**
* @Methog: DecodeIDToken
* @Author: Wayan Agus Yudiana
* @Description: This method is validating the input token in header Authorization to the Firebase Authenticaiton
* @Input: Token 
* @Output: Decoded token
*/
async function decodeIDToken(req, res) 
{

 
    if(!req.headers.authorization) 
    {
        return null;
    }

    const authHeader = req.headers.authorization;

    if(authHeader) 
    {
       try
        {
            const token = authHeader.split('Bearer ')[1];
            //printLog(`TOKEN: ${token}`);
    
            var dcodeToken="";
            await admin.auth().verifyIdToken(token).then((decodedToken) =>
            {   
                dcodeToken = JSON.parse(JSON.stringify(decodedToken));
                return null;

               // console.log(JSON.stringify(dcodeToken));
            }).catch((error) =>
            {
               printLog(`TOKEN VALIDATION: ${error}`);
               return null;
            });

            //console.log(JSON.stringify(dcodeToken));
            return JSON.parse(JSON.stringify(dcodeToken));

        }
        catch(error)
        {
            //const token2 = authHeader.split('Bearer ')[1];
            return null;
        }
        
   } 
  else 
  {
        printLog(`EMPTY HEADER`);
        return null;
       
  }
 
}

/**
* @Methog: Login Handler 
* @Author: Wayan Agus Yudiana
* @Description: This handler executed when client request to login using their registered account,
* at your firebase project
* @Input: email and password which are passed in the request data
* @Output: response token to the requestor
*/
async function loginHandler(req, res) {

    try{

        var email = req.body.email;
        var pass = req.body.pass;
  
        await auth.signInWithEmailAndPassword(email, pass).then((userdata) =>  
        {
            
            printLog(`LOGIN.....`);
            user=userdata;
            return null;

        })
        .catch((err)=>
        {
                printLog(err);
                res.writeHead(500, {"Content-Type": "application/json"});
                res.end(JSON.stringify({error:err}));
                return null;
        });

        if (!user) { 

            printLog(`ERROR LOGIN`);
            respond.status(403).send('You must be logged in!');
            return null;
        }
         else
        {

            auth.currentUser.getIdToken(true).then((token) => 
            {
                    printLog(`GENERATE TOKEN.....`);
                    res.writeHead(200, {"Content-Type": "application/json"});
                    //res.end(JSON.stringify({token:token}));
                    res.end(`${token}\n\n`);
                    return null;

            }).catch((error) => {

                   res.writeHead(500, {"Content-Type": "application/json"});
                   res.end(JSON.stringify({error:error}));
                   return null;

            });
        }
        return null;

    }catch(err)
    {
        printLog(err);
        res.writeHead(500, {"Content-Type": "application/json"});
        res.end(JSON.stringify({error:err}));
        return null;

    }
     
}

/**
* @Methog: TokenValidationHandler 
* @Author: Wayan Agus Yudiana
* @Description: This handler executed when client request to to validate token from the registered account.
* @Input: Token in  header Authorization field.
* @Output: Response 200 when token is valid.
*/
async function tokenValidationHandler(request, respond) {

    await decodeIDToken(request, respond).then((decodedToken) =>
    {
        //printLog(decodedToken);
        user=decodedToken;
        printLog(`CLIENT REGISTERED WITH USER ID: ${user.user_id} EMAL: ${user.email}`);
        return null;

    }).catch((error) =>
    {
        printLog(`TOKEN VALIDATION: ${error}`);
        return null;
    });
    
    console.log("=========================================");


    if (!user) { 

            printLog(`ERROR LOGIN`);
            respond.status(403).send('Token Not Valid!');
        }
    else
    {
        printLog(`GENERATE TOKEN.....`);
       respond.status(200).send('Token is Valid!');

    }

}



/**
* @Methog: Login URL Routing  
* @Author: Wayan Agus Yudiana
* @Description: The endpoint router for login  
* @Input: email and password which are passed in the request data
* @Output: response token to the requestor
*/
app.post('/login',loginHandler);

/**
* @Methog: Token Validation URL Routing  
* @Author: Wayan Agus Yudiana
* @Description: The endpoint router for token validation  
* @Input: token which are passed in the Authorization header
* @Output: response 200 when it success or 403 when it fail
*/
app.post('/validatetoken',tokenValidationHandler);




// Start server, For deploy to firebase  comment this line before deployment 
//app.listen(port, () => console.log(new Date()+`|SSE Server  listening on port ${port}`));

// Expose Express API as a single Cloud Function:
exports.app = functions.https.onRequest(app);




/**
 * Decodes the JSON Web Token sent via the frontend app
 * Makes the currentUser (firebase) data available on the body.
 */
/*

const database  = firebase.database();

//test with automatic even
//const WAITTIMER = 10000;
//setInterval(sendStreamToAllClients, WAITTIMER);



async function decodeIDToken2(req, res, next) 
{

 
  if(!req.headers.authorization) 
    {
        printLog(`========Verify Token==========`);
        return next();
    }

  const authHeader = req.headers.authorization;

   //console.log(authHeader.split('Bearer ')[1]);

  if (authHeader) 
  {
       try
        {
            const token = authHeader.split('Bearer ')[1];
            //console.log(`TOKEN: ${token}`);

            
            await admin.auth().verifyIdToken(token).then((decodedToken) =>
            {
                let options = {
                    maxAge: 1000 * 60 * 15, // would expire after 15 minutes
                    httpOnly: false, // The cookie only accessible by the web server
                    signed: false // Indicates if the cookie should be signed
                }


                req['currentUser'] = decodedToken;
                //console.log(decodedToken);
                //res.cookie('tokendata',decodedToken , options) // options is optional
                //res.send('')
                //next();
                return

                // ...
            }).catch((error) =>
            {
               printLog(`TOKEN VALIDATION: ${error}`);
               return next();
            });
        }
        catch(error)
        {
            const token2 = authHeader.split('Bearer ')[1];
            return next();
        }
        
   } 
  else 
  {
        printLog(`EMPTY HEADER`);
        return next();
       
  }

return next();
 
}




var userIndex = 0
var clients = [];



//url for the client request submition ot the stream data.
app.get('/requeststreams', subscriptionHandler);

// This handler executed when client request listenning the stream
async function subscriptionHandler(request, respond) {

    //decodeIDToken(request, respond);
    request.socket.setTimeout(15*60*1000);
    
    await decodeIDToken(request, respond).then((decodedToken) =>
    {
        //printLog(decodedToken);
        user=decodedToken;
        printLog(`CLIENT REGISTERED WITH USER ID: ${user.user_id} EMAL: ${user.email}`);
        return null;

    }).catch((error) =>
    {
        printLog(`TOKEN VALIDATION: ${error}`);
        return null;
    });
    
    console.log("=========================================");


    if (!user) { 

            printLog(`ERROR LOGIN`);
            respond.status(403).send('You must be logged in!');
        }
     else
    {
            const clientId = Date.now();

            printLog(`START LEASTENING EVENT BY CLIENT [${clientId}] ${user.email}`);
            printLog(`=========================LISTENING EVENT======================`);
            //return header with 200 statu code and header value
            respond.writeHead(200, headerData);

            //format the body of the data stream
            var now=getDateNow();

            let firstStreamData = {
                'time': `${now}`,
                'msg': 'Stream data for the listener ['+ user.email +']'
            }

            //generate new client data which its index and respond data
            const newClient = {
                index : userIndex,
                id: clientId,
                respond
              };
            clients.push(newClient);

            //Increment index of the client
            userIndex++;

            //Remove client when they close the  connection
            request.on('close', () => {
                printLog(`[${clientId}][${user.email}] Connection closed`);
                clients = clients.filter(c => c.index !== userIndex);
            });





        }
    


}





var commandData  = database.ref("data/");
async function refreshDatabaseRefLogin() {

        var email = "[your registerd email]";
        var pass = "[your registerd pass]]";
        //var userdata;

        await auth.signInWithEmailAndPassword(email, pass).then((userdata) =>  
        {
            
            printLog(`GET DB REF TOKEN.....`);
            user=userdata;
            return null;

        })
        .catch((err)=>
        {
                printLog(err);
                return null;
        });

        if (!user) { 

            printLog(`ERROR LOGIN DB REF.....`);
           
        }
         else
        {
            printLog(`LOGIN DB REF SUCESS.....`);
            commandData  = database.ref("data/");
            commandData.on('value', (snapshot) => {

                var now=getDateNow();
                let streamData = {
                    'time': `${now}`,
                    'cmd': "setTemperature",
                    'params' : '{\"temp\":80,\"effect\":\"smooth\",\"duration\":1500}'
                }

                printLog(`DB CHANGE, SEND INFO TO ALL CLIENT.....`);
                 //Broadcast the data to all listed client in json database
                clients.forEach(c => c.respond.write(`data: ${JSON.stringify(streamData)}\n\n`));

            });


        }

         return null;

}

//const WAITTIMER = 100000;
//setInterval(refreshDatabaseRefToken, WAITTIMER);

refreshDatabaseRefLogin();


//push an event to server an respond ok to all subscribed clients
app.post('/sendevent', sendEventHandler);







// This handler executed when client request listenning the stream
async function sendEventHandler(request, respond) {

    //decodeIDToken(request, respond);
    
    await decodeIDToken(request, respond).then((decodedToken) =>
    {
        user=decodedToken;
        printLog(`SEND EVENT BY CLIENT [${user.user_id}] EMAL: [${user.email}]`);
        return  null;

    }).catch((error) =>
    {
        printLog(`TOKEN VALIDATION: ${error}`);
        return null;
    });

    //console.log("=========================================");

    if (!user) { 
        printLog(`LOGIN ERROR`);
        respond.status(403).send('You must be logged in!\n\n');
        }
        else
        {
            try
            {

                var cmd = request.body.cmd;
                var params = request.body.params;

                //console.info(cmd);
                //console.info(params);

                sendStreamToAllClients(cmd,params);
                respond.writeHead(200);
                respond.end('Yor events has sucessfuly been published!\n\n');
            }
           catch(error)
           {
                console.error(error);
                respond.status(403).send('You must be logged in!');
            }

        }

    
}


// Send events to all clients using SSE
function sendStreamToAllClients(cmd,params) {

    //console.info(new Date()+'|Sending stream data to all clients, this is '+str+' event');

    var now=getDateNow();
    let streamData = {
        'time': `${now}`,
        'cmd': cmd,
        'params' : params
    }

    //Broadcast the data to all listed client in json database
    clients.forEach(c => c.respond.write(`data: ${JSON.stringify(streamData)}\n\n`))

}



****/
