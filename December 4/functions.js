function LogInButtonClick() 
{
  var x = document.getElementById("edit");
  x.innerHTML = "<b>Log In</b><br><form method='post' action='login'>Username: <input type='text' name='username'><br>Password:  <input type='password' name='password'><br><input type = 'submit' value = 'Submit'></form>";
}
function InvitationsButtonClick()
{
  var x = document.getElementById("edit");
  x.innerHTML = "You don't have any event invitations. But don't feel bad. It's not necessarily because nobody likes you. The invitations system isn't even implemented yet!";
}
function ManageEventsButtonClick()
{
  var x = document.getElementById("edit");
  x.innerHTML = "You have no events scheduled at this time.";
}
function CreateEventButtonClick()
{
  var x = document.getElementById("edit");
   x.innerHTML = "<div id='createeventdiv'><button id='eventwithfriendsbutton' onclick='EventWithFriendsButtonClick()'>Event With Friends</button><form method='get' action='createEvent'><table><tr><td>Event name:</td><td> <input type='text' name='eventname'></td></tr><tr><td>Date:</td><td> <input type='date' name='eventdate'></td></tr><tr><td>Start time:</td><td> <input type='time' name='eventstarttime'></td></tr><tr><td>End time:</td><td> <input type='time' name='eventendtime'></td></tr><tr><td>Repeat every: </td><td><input type='checkbox' name='Sunday'>Sun<input type='checkbox' name='Monday'>Mon<input type='checkbox' name='Tuesday'>Tues<input type='checkbox' name='Wednesday'>Wed<input type='checkbox' name='Thursday'>Thurs<input type='checkbox' name='Friday'>Fri<input type='checkbox' name='Saturday'>Sat</td></tr><tr><td>Attendance likelihood:</td><td> <input type='range' name='likelihood'></td></tr></table><input type='submit' value='Create Event'></form></div>";
}
function FriendsButtonClick()
{
  var x = document.getElementById("edit");
  x.innerHTML = "<p>You don't have any friends.</p><br><br><br><br><br><br><button id='friendinvitationsbutton' onclick='FriendInvitationsButtonClick()'>Friend Invitations</button><button id='addfriendbutton' onclick='AddFriendButtonClick()'>Add Friend</button>";
}
function LogOutButtonClick()
{
  var x = document.getElementById("edit");
  x.innerHTML = "You're already logged out, so I guess this one technically works by default.";
}
function AddFriendButtonClick()
{
	var x = document.getElementById("edit");
	x.innerHTML = "<p>Add Friend</p><br><form action='frinvitationSend' method='get'>Username: <input type='text' name='addfriendusername'><br><input type='submit' value='Send'></form><form action='friends' method='get'><input type='submit' value='Back'></form>";
}
function AddFriendBackButtonClick()
{
	FriendsButtonClick();
}

function FriendInvitationsButtonClick()
{
	var x = document.getElementById("edit");
	x.innerHTML = "<p>You have no friend invitations at this time.</p><br><br><br><br><br><br><br><form action='friends' method='get'><input type='submit' value='Back'></form>"
}

function EventWithFriendsButtonClick()
{
	var x = document.getElementById("edit");
	x.innerHTML = "<p>Event With Friends</p><form>Event Name: <input type='text' name='eventnamewithfriends'><br>Duration: <input type='text' name='duration'><br>Earliest Start: <input type='text' name='earlieststart'><br>Latest Start: <input type='text' name='lateststart'><br>Date range: <input type='text' name='daterange'><br>Friends invited: <input type='text' name='friendsinvited'><br><input type='submit' value='Create Event'></form><button id = 'backbuttons' onclick='CreateEventButtonClick()'>Back</button>"
}