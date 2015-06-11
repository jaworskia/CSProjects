function setEditPos(x)
{document.getElementById("edit").style.top=x;}

function buttonParse(btn)
{
  setEditPos("50px");  
  switch(btn){
    case 1: /*invite*/
    setTimeout(function(){ajaxEnvitations();},500);
    break;
    case 2: /*manage*/
    setTimeout(function(){ajaxManageEvents();},500);
    case 3: /*create*/
    setTimeout(function(){CreateEventButtonClick();},500);               
    break;
    case 4: /*friend*/
    setTimeout(function(){ajaxFriends();},500);              
    break;  
    case 5:
    setTimeout(function(){AddFriendButtonClick();},500);
    break;
    case 6:
    setTimeout(function(){ajaxFrinvitations();},500);
    break;
    case 7:
    setTimeout(function(){ajaxAccept();},500);
    break;
    case 8:
    setTimeout(function(){ajaxIgnore();},500);
    break;
  }
    setTimeout(function(){setEditPos("400px");},500);
   //document.getElementById("edit").style.zIndex=4;
   //document.getElementById("edit").style.visibility="visible";
   //document.getElementById("edit").style.transition-delay=".5s";
   //document.getElementById("edit").style.top="400px";
   //document.getElementById("edit").style.transition-delay="0s";
}

function LogInButtonClick() 
{
  var x = document.getElementById("info");
  x.innerHTML = "<b>Log In</b><br><form method='post' action='login'>Username: <input type='text' name='username'><br>Password:  <input type='password' name='password'><br><input type = 'submit' value = 'Submit'></form>";
}
function InvitationsButtonClick()
{
  var x = document.getElementById("info");
  x.innerHTML = "You don't have any event invitations. But don't feel bad. It's not necessarily because nobody likes you. The invitations system isn't even implemented yet!";
}
function ManageEventsButtonClick()
{
  var x = document.getElementById("info");
  //x.innerHTML = "You have no events scheduled at this time.";
}
function CreateEventButtonClick()
{
  var x = document.getElementById("info");
   x.innerHTML = "<div id='createeventdiv'><button id='eventwithfriendsbutton' onclick='ajaxEventWithFriends()'>Event With Friends</button><form method='get' action='createEvent'><table><tr><td>Event name:</td><td> <input type='text' name='eventname'></td></tr><tr><td>Date:</td><td> <input type='date' name='eventdate'></td></tr><tr><td>From:</td><td> <input type='time' name='eventstarttime'></td><td>To:</td><td> <input type='time' name='eventendtime'></td></tr><tr><td>Repeat every: </td><td><input type='checkbox' name='Sunday'>Sun<input type='checkbox' name='Monday'>Mon<input type='checkbox' name='Tuesday'>Tues<input type='checkbox' name='Wednesday'>Wed<input type='checkbox' name='Thursday'>Thurs<input type='checkbox' name='Friday'>Fri<input type='checkbox' name='Saturday'>Sat</td></tr><tr><td>Attendance likelihood:</td><td> <input type='range' name='likelihood'></td></tr></table><input type='submit' value='Create Event'></form></div>";
}
function FriendsButtonClick()
{
  var x = document.getElementById("info");
  //x.innerHTML = "<p>You don't have any friends.</p><br><br><br><br><br><br><button id='friendinvitationsbutton' onclick='FriendInvitationsButtonClick()'>Friend Invitations</button><button id='addfriendbutton' onclick='AddFriendButtonClick()'>Add Friend</button>";
}
function LogOutButtonClick()
{
  var x = document.getElementById("info");
  x.innerHTML = "You're already logged out, so I guess this one technically works by default.";
}
function AddFriendButtonClick()
{
	var x = document.getElementById("info");
	x.innerHTML = "<p>Add Friend</p><br>Username: <input type='text' id='addfriendname'><button onclick='ajaxSendFrinvitation()'>Send</button><br><button onclick='buttonParse(4)'>Back</button>";
}
function AddFriendBackButtonClick()
{
	FriendsButtonClick();
}

function FriendInvitationsButtonClick()
{
	var x = document.getElementById("info");
	x.innerHTML = "<p>You have no friend invitations at this time.</p><br><br><br><br><br><br><br><form action='friends' method='get'><input type='submit' value='Back'></form>"
}

function EventWithFriendsButtonClick()
{
	var x = document.getElementById("info");
	x.innerHTML = "<form><table id='withFriendsTable'><tr><td>Event Name:</td><td><input type='text' name='eventnamewithfriends'><br></td></tr><tr><td>Duration:</td><td><input type='text' name='duration'><br></td></tr><tr><td>Earliest Start:</td><td><input type='text' name='earlieststart'><br></td></tr><tr><td>Latest Start:</td><td><input type='text' name='lateststart'><br></td></tr><tr><td>Date range:</td><td><input type='text' name='daterange'><br></td></tr><tr><td>Friends invited:</td><td><input type='text' name='friendsinvited'><br></td></tr></table><input type='submit' value='Create Event'></form><button id = 'smallbutton' onclick='CreateEventButtonClick()'>Back</button>"
}