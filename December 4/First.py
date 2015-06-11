import webapp2
import os
import random

from google.appengine.ext.webapp import template
from google.appengine.api import users
from google.appengine.ext import db
from google.appengine.api import memcache

class Event(db.Model):
  name = db.StringProperty()
  date = db.StringProperty()
  start = db.StringProperty()
  end = db.StringProperty()
  sun = db.StringProperty()
  mon = db.StringProperty()
  tues = db.StringProperty()
  wed = db.StringProperty()
  thurs = db.StringProperty()
  fri = db.StringProperty()
  sat = db.StringProperty()
  attendance = db.StringProperty()
  
class EWFHelper():      #Event With Friends Helper class; stores data concerning each friend
  name = ''
  invited = ''
  priority = 0
  availabilities = {}
  def __init__(self, name, invited, priority):
    self.name = name
    self.invited = invited
    self.priority = priority
    
class Increment():
  date = ''
  time = ''
  def __init__(self, date, time):
    self.date = date
    self.time = time
    
class Option():
  date = ''
  time = ''
  finished = ''
  score = 0
  def __init__(self, date, time, finished, score):
    self.date = date
    self.time = time
    self.finished = finished
    self.score = score
  
  
class User(db.Model):
  name = db.StringProperty()
  
class Friend(db.Model):
  name = db.StringProperty()
  
class Envitation(db.Model):
  sender = db.StringProperty()
  date = db.StringProperty()
  start = db.StringProperty()
  end = db.StringProperty()
  eName = db.StringProperty()
  
class Frinvitation(db.Model):
  sender = db.StringProperty()
    
class MainPage(webapp2.RequestHandler) :
      #the first thing that happens
  def get(self) :
    user = users.get_current_user()
    if user:
      current = User(key_name=user.nickname())
      current.name = user.nickname()
      current.put()
      nickname = 'Well hello there, ' + user.nickname() + '!'
      template_values = {'message' : nickname, 'indexNumber1' : 1}
      self.response.out.write(template.render('HTML/newIndex.html', template_values))
    else:
      self.redirect(users.create_login_url(self.request.uri))
      
class CreateEvent(webapp2.RequestHandler) :     #runs when you click the 'Create Event' button within the sidebar Create Event button
  def get(self) :
    user = users.get_current_user()                #get the current user
    if not user:
      self.redirect(users.create_login_url(self.request.uri))
    name = self.request.get('eventname')          #get all the passed-in values
    date = self.request.get('eventdate')
    start = self.request.get('eventstarttime')
    end = self.request.get('eventendtime')
    attendance = self.request.get('likelihood')
    sunday = self.request.get('Sunday')
    monday = self.request.get('Monday')
    tuesday = self.request.get('Tuesday')
    wednesday = self.request.get('Wednesday')
    thursday = self.request.get('Thursday')
    friday = self.request.get('Friday')
    saturday = self.request.get('Saturday')
    if (not name or not date or not start or not end):
      thing = 'Event Incomplete. Event Not Created.'
      template_values = {'message': thing, 'indexNumber1': 1}
      self.response.out.write(template.render('HTML/newIndex.html', template_values))
    elif ("'" in name):
      thing = "Event not created. Event name must not contain an apostrophe"
      template_values = {'message': thing, 'indexNumber1': 1}
      self.response.out.write(template.render('HTML/newIndex.html', template_values))
    elif (">" in name):
      thing = "Event not created. Event name must not contain a greater than symbol"
      template_values = {'message': thing, 'indexNumber1' : 1}
      self.response.out.write(template.render('HTML/newIndex.html', template_values))
    else:
      stupidDate = verifyDateIsNotStupid(date)
      stupidStart = verifyTimeIsNotStupid(start)
      stupidEnd = verifyTimeIsNotStupid(end)
      stupidAttendance = verifyAttendanceIsNotStupid(attendance)
      if (stupidDate == "stupid"):
        thing = "Date format improper. Proper format for dates is: YYYY-MM-DD"
        template_values = {'message': thing, 'indexNumber1': 1}
        self.response.out.write(template.render('HTML/newIndex.html', template_values))
      elif (stupidStart == "stupid" or stupidEnd == "stupid"):
        thing = "Time format improper. Proper format for times is: HH:MM"
        template_values = {'message': thing, 'indexNumber1': 1}
        self.response.out.write(template.render('HTML/newIndex.html', template_values))
      elif (stupidAttendance == "stupid"):
        thing = "Attendance format improper. Attendance must be an integer 0 - 100"
        template_values = {'message': thing, 'indexNumber1': 1}
        self.response.out.write(template.render('HTML/newIndex.html', template_values))
      elif (start >= end):
        thing = "Event not created. Start time must be before end time."
        template_values = {'message': thing, 'indexNumber1': 1}
        self.response.out.write(template.render('HTML/newIndex.html', template_values))
      else:
        current_k = db.Key.from_path('User', user.nickname())  #get the current user's key
        current = db.get(current_k)          #get the db entity corresponding to the current user
        thekey = current.key()              #get the current user's key...possibly equivalent to two lines up
        event = Event(parent=thekey)        #so that this event will be a child of the current user
        event.name = name                    #set the event's properties
        event.date = date
        event.start = start
        event.end = end
        event.attendance = attendance
        event.sun = sunday
        event.mon = monday
        event.tues = tuesday
        event.wed = wednesday
        event.thurs = thursday
        event.fri = friday
        event.sat = saturday
        event.put()                    #add the event to the database
        thing = 'Event Created'        #just a little message confirming it worked
        template_values = {'message' : thing, 'indexNumber1' : 1}
        self.response.out.write(template.render('HTML/newIndex.html', template_values))  #give back the main page again
    
class LogIn(webapp2.RequestHandler) :
        #never happens, as the log in button no longer exists
  def post(self) :
    self.response.out.write('Hey, you tried to log in!')
    
class ManageEvents(webapp2.RequestHandler):      #when you click the 'Manage Events' button in the sidebar
  def get(self):
    user = users.get_current_user()
    current_k = db.Key.from_path('User', user.nickname())  #get the current user's key
    current = db.get(current_k)          #get the db entity corresponding to the current user
    thekey = current.key()              #get the current user's key...possibly equivalent to two lines up
    q = Event.all()                      #q is now all Event entities
    q.ancestor(thekey)                    #q is now filtered to only be Event entities of the current user
    thing = "Events:\n"
   # for p in q:                          #for each of the user's events
     # thing += (p.name +"\n")
   # event_k = db.Key.from_path('User', user.nickname(), 'Event', 1)
   # event = db.get(event_k)
   # thing = event.name
    template_values = {'events' : q, 'indexNumber2' : 2}        #we'll pass the user's events to the template
    self.response.out.write(template.render('HTML/newIndex.html', template_values))  #now we write to the index2 template
    
class DeleteEvent(webapp2.RequestHandler):      #when you click a delete event button from within 'Manage Events'
  def get(self):
    thekey = self.request.get('thekey')          #thekey (key for the Event entity) is passed in with a hidden input in index2.html
    db.delete(thekey)                            #delete it
    thing = 'Event Deleted'
    template_values = {'message' : thing, 'indexNumber1' : 1}
    self.response.out.write(template.render('HTML/newIndex.html', template_values))  #back to index.html
    
class Friends(webapp2.RequestHandler):          #when you click the 'Friends' button in the sidebar (or one of the 'Back' buttons)
  def get(self):
    user = users.get_current_user()
    current_k = db.Key.from_path('User', user.nickname())  #get the current user's key
    current = db.get(current_k)          #get the db entity corresponding to the current user
    thekey = current.key()              #get the current user's key...possibly equivalent to two lines up
    q = Friend.all()                      #q is now all Friend entities
    q.ancestor(thekey)                    #q is now filtered to only be Friend entities of the current user
    thing = 'You hit the Friends button'
    template_values = {'friends' : q, 'indexNumber3' : 3}
    self.response.out.write(template.render('HTML/newIndex.html', template_values))
    
class FrinvitationSend(webapp2.RequestHandler):    #when you send a friend invitation
  def get(self):
    recipient = self.request.get('addfriendusername')
    recipient_k = db.Key.from_path('User', recipient)
    thing = recipient_k
    derp = db.get(recipient_k)
    if (derp):
      user = users.get_current_user()
      if (recipient == user.nickname()):
        template_values = {'message' : "You can't be friends with yourself", 'indexNumber1' : 1}
        self.response.out.write(template.render('HTML/newIndex.html', template_values))
      frinvitation = Frinvitation(parent=recipient_k, key_name = user.nickname())  #so that this Frinvitation will be a child of the recipient
      frinvitation.sender = user.nickname()
      frinvitation.put()
      template_values = {'message' : 'Invitation sent to ' + recipient, 'indexNumber1' : 1}
      self.response.out.write(template.render('HTML/newIndex.html', template_values))
    else:
      template_values = {'message' : recipient + ' is not a valid username', 'indexNumber1' : 1}
      self.response.out.write(template.render('HTML/newIndex.html', template_values))
    
class DisplayFrinvitations(webapp2.RequestHandler):    #when you click the 'Friend Invitations' button
  def get(self):
    user = users.get_current_user()
    current_k = db.Key.from_path('User', user.nickname())  #get the current user's key
    current = db.get(current_k)          #get the db entity corresponding to the current user
    thekey = current.key()              #get the current user's key...possibly equivalent to two lines up
    q = Frinvitation.all()                      #q is now all Frinvitation entities
    q.ancestor(thekey)                    #q is now filtered to only be Frinvitation entities of the current user
    thing = 'Here are your friend invitations'
    template_values = {'frinvitations' : q, 'indexNumber4' : 4}
    self.response.out.write(template.render('HTML/newIndex.html', template_values))
    
class Accept(webapp2.RequestHandler):
  def get(self):
    user = users.get_current_user()
    sender = self.request.get('sender')
    thekey = self.request.get('thekey')
    sender_k = db.Key.from_path('User', sender)
    recipient_k = db.Key.from_path('User', user.nickname())
    senderObject = Friend(parent=sender_k, key_name = user.nickname())
    senderObject.name = user.nickname()
    senderObject.put()
    recipientObject = Friend(parent=recipient_k, key_name = sender)
    recipientObject.name = sender
    recipientObject.put()
    db.delete(thekey)
    thing = 'Invitation Accepted'
    template_values = {'message' : thing, 'indexNumber1' : 1}
    self.response.out.write(template.render('HTML/newIndex.html', template_values))
    
class Ignore(webapp2.RequestHandler):
  def get(self):
    thekey = self.request.get('thekey')    #thekey (key for the Frinvitation entity) is passed in with a hidden input in index4.html
    db.delete(thekey)                            #delete it
    thing = 'Wow what a jerk'
    template_values = {'message' : thing, 'indexNumber1' : 1}
    self.response.out.write(template.render('HTML/newIndex.html', template_values))
    
class Unfriend(webapp2.RequestHandler):
  def get(self):
    user = users.get_current_user()
    name2 = self.request.get('name2')
    thekey = self.request.get('thekey')
    friend_k = db.Key.from_path('User', name2, 'Friend', user.nickname())
    db.delete(thekey)
    db.delete(friend_k)
    thing = name2 + ' has been unfriended'
    template_values = {'message' : thing, 'indexNumber1' : 1}
    self.response.out.write(template.render('HTML/newIndex.html', template_values))
                            
class LogOut(webapp2.RequestHandler) :
        #when you click the 'Log Out' button in the sidebar
  def post(self) :
    self.redirect(users.create_logout_url('/'))
    
class AjaxManageEvents(webapp2.RequestHandler):
  def post(self):
    user = users.get_current_user()
    text = ''
    current_k = db.Key.from_path('User', user.nickname())  #get the current user's key
    current = db.get(current_k)          #get the db entity corresponding to the current user
    thekey = current.key()              #get the current user's key...possibly equivalent to two lines up
    target = self.request.get('target')
    q = Event.all()                      #q is now all Event entities
    q.ancestor(thekey)
    allEvents = 0
    if (not (target == 'null')):          #if there's a target
      #q.filter("date =", target)          #also filter the events by the target date
      text += "Events for: " + target
    else:
      text += "All Events"
      allEvents = 1
    text += '<table id="manageTable"><tr><td>          </td><td>Date</td><td>Start</td><td>End</td><td>Likelihood</td><td>Repeat</td><td class="end">Delete</td></tr>'
    for p in q:
      if (eventOccursOnDate(p, target) == "yes" or allEvents == 1): #if the event and the target date overlap (taking repeats into consideration)
        text += ('<tr>')
        text += ('<td><b>' + p.name + '</b></td></tr><tr><td></td>')
        text += ('<td>' + p.date + '</td>')
        text += ('<td>' + p.start + '</td>')
        text += ('<td>' + p.end + '</td>')
        text += ('<td>' + p.attendance + '%</td>')
        text += ('<td>')
        if p.sun:
          text += 'Su'
        if p.mon:
          text += 'Mo'
        if p.tues:
          text += 'Tu'
        if p.wed:
          text += 'We'
        if p.thurs:
          text += 'Th'
        if p.fri:
          text += 'Fr'
        if p.sat:
          text += 'Sa'
        text += '</td>'
        key2 = str(p.key())
        text += '<td class="end"><button onclick="ajaxDeleteEvent('
        text += '\'' + key2 + '\''
        text += ')">Delete</button></td>'
        #text += ('<button onclick=ajaxDeleteEvent(' + key2 + ')>Delete Event</button>')
        text += '</tr>'
    text +='</table>'    
    self.response.out.write(text)
    
class AjaxDeleteEvent(webapp2.RequestHandler):
  def post(self):
    thekey = self.request.get('thekey')
    db.delete(thekey)
    self.response.out.write('Event Deleted')
    
class AjaxFriends(webapp2.RequestHandler):
  def post(self):
    text = ''
    user = users.get_current_user()
    current_k = db.Key.from_path('User', user.nickname())  #get the current user's key
    current = db.get(current_k)          #get the db entity corresponding to the current user
    thekey = current.key()              #get the current user's key...possibly equivalent to two lines up
    q = Friend.all()                      #q is now all Friend entities
    q.ancestor(thekey)                    #q is now filtered to only be Friend entities of the current user
    text += '<button onclick="buttonParse(6)">Friend Invitations</button>'
    text += '<button onclick="buttonParse(5)">Add Friend</button>'
    for p in q:
      text += ('<p>' + p.name + '</p>')
      key2 = str(p.key())
      text += '<button onclick="ajaxUnfriend('
      text += '\'' + key2 + '\','
      text += '\'' + p.name + '\''
      text += ')">Unfriend</button>'
    self.response.out.write(text)
    
class AjaxUnfriend(webapp2.RequestHandler):
  def post(self):
    user = users.get_current_user()
    thekey = self.request.get('thekey')
    name2 = self.request.get('name2')
    friend_k = db.Key.from_path('User', name2, 'Friend', user.nickname())
    db.delete(thekey)
    db.delete(friend_k)
    self.response.out.write(name2 + " has been unfriended")
    
class AjaxSendFrinvitation(webapp2.RequestHandler):
  def post(self):
    recipient = self.request.get('input')
    recipient_k = db.Key.from_path('User', recipient)
    user = users.get_current_user()
    current_k = db.Key.from_path('User', user.nickname())  #get the current user's key
    current = db.get(current_k)          #get the db entity corresponding to the current user
    thekey = current.key()              #get the current user's key...possibly equivalent to two lines up
    thing = recipient_k
    derp = db.get(recipient_k)
    alreadyFriends = 0
    q = Friend.all()                      #q is now all Friend entities
    q.ancestor(thekey)                    #q is now filtered to only be Friend entities of the current user
    for p in q:
      if (recipient == p.name):
        alreadyFriends = 1
    if (derp):
      if (recipient == user.nickname()):
        self.response.out.write("You can't be friends with yourself")
      elif (alreadyFriends == 1):
        self.response.out.write("You are already friends with " + recipient)
      else:
        frinvitation = Frinvitation(parent=recipient_k, key_name = user.nickname())  #so that this Frinvitation will be a child of the recipient
        frinvitation.sender = user.nickname()
        frinvitation.put()
        self.response.out.write("Invitation sent to: " + recipient)
    else:
      if (">" in recipient):
        self.response.out.write("Recipient must not contain a greater than symbol")
      else:
        self.response.out.write(recipient + " is not a valid username")
      
class AjaxFrinvitations(webapp2.RequestHandler):
  def post(self):
    user = users.get_current_user()
    current_k = db.Key.from_path('User', user.nickname())  #get the current user's key
    current = db.get(current_k)          #get the db entity corresponding to the current user
    thekey = current.key()              #get the current user's key...possibly equivalent to two lines up
    q = Frinvitation.all()                      #q is now all Frinvitation entities
    q.ancestor(thekey)                    #q is now filtered to only be Frinvitation entities of the current user
    text = ''
    for p in q:
      text += ('<p>' + p.sender + '</p>')
      key2 = str(p.key())
      text += '<button onclick="ajaxAccept('
      text += '\'' + key2 + '\','
      text += '\'' + p.sender + '\''
      text += ')">Accept</button>'
      text += '<button onclick="ajaxIgnore('
      text += '\'' + key2 + '\''
      text += ')">Ignore</button>'
    text += '<button onclick="buttonParse(4)">Back</button>'
    self.response.out.write(text)
    
class AjaxAccept(webapp2.RequestHandler):
  def post(self):
    user = users.get_current_user()
    sender = self.request.get('sender')
    thekey = self.request.get('thekey')
    sender_k = db.Key.from_path('User', sender)
    recipient_k = db.Key.from_path('User', user.nickname())
    senderObject = Friend(parent=sender_k, key_name = user.nickname())
    senderObject.name = user.nickname()
    senderObject.put()
    recipientObject = Friend(parent=recipient_k, key_name = sender)
    recipientObject.name = sender
    recipientObject.put()
    db.delete(thekey)
    self.response.out.write('Invitation Accepted')
    
class AjaxIgnore(webapp2.RequestHandler):
  def post(self):
    thekey = self.request.get('thekey')
    db.delete(thekey)
    self.response.out.write('Request Ignored')
    
class AjaxEventWithFriends(webapp2.RequestHandler):  #sets up the EventWithFriends inputs and whatnot
  def post(self):
    user = users.get_current_user()
    current_k = db.Key.from_path('User', user.nickname())  #get the current user's key
    current = db.get(current_k)          #get the db entity corresponding to the current user
    thekey = current.key()              #get the current user's key...possibly equivalent to two lines up
    q = Friend.all()                      #q is now all Friend entities
    q.ancestor(thekey)                    #q is now filtered to only be Friend entities of the current user
    text = ''
    text += "<button onclick='CreateEventButtonClick()'>Back</button>"  #back to Create Event page
    text += "<form method='post' action='createEventWithFriends'>"      #form with action for handler on line 433
    text += "<table>"
    text += "<tr><td>Event name:</td>"
    text += "<td><input type='text' name='eventName'></td></tr>"
    text += "<tr><td>Duration in hours:</td>"
    text += "<td><select name='hours'>"                            #discrete options for hours, 0-23
    text += "<option value=0>0</option>"
    text += "<option value=1 selected>1</option>"
    text += "<option value=2>2</option>"
    text += "<option value=3>3</option>"
    text += "<option value=4>4</option>"
    text += "<option value=5>5</option>"
    text += "<option value=6>6</option>"
    text += "<option value=7>7</option>"
    text += "<option value=8>8</option>"
    text += "<option value=9>9</option>"
    text += "<option value=10>10</option>"
    text += "<option value=11>11</option>"
    text += "<option value=12>12</option>"
    text += "<option value=13>13</option>"
    text += "<option value=14>14</option>"
    text += "<option value=15>15</option>"
    text += "<option value=16>16</option>"
    text += "<option value=17>17</option>"
    text += "<option value=18>18</option>"
    text += "<option value=19>19</option>"
    text += "<option value=20>20</option>"
    text += "<option value=21>21</option>"
    text += "<option value=22>22</option>"
    text += "<option value=23>23</option>"
    text += "</select></td>"
    text += "<td>Duration in minutes:</td>"
    text += "<td><select name='minutes'>"
    text += "<option value=0 selected>0</option>"      #I'm thinking 15 minutes is a good interval
    text += "<option value=15>15</option>"
    text += "<option value=30>30</option>"
    text += "<option value=45>45</option>"
    text += "</select></td></tr>"
    text += "<tr><td>Earliest start:</td>"
    text += "<td><input type='time' name='earliestStart'></td>"    #various other inputs
    text += "<td>Latest start:</td>"
    text += "<td><input type='time' name='latestStart'></td></tr>"
    text += "<tr><td>Between</td>"
    text += "<td><input type='date' name='first'><td>"
    text += "<td>and</td>"
    text += "<td><input type='date' name='last'></td></tr>"
    text += "<tr><td>Friends to invite:</td></tr>"
    for p in q:
      text += "<tr><td>" + p.name + "</td>"  #gives a checkbox and a priority select menu for each friend
      text += "<td><input type='checkbox' name='" + p.name + "' value='yes'></td>"
      text += "<td>Priority:</td>"
      text += "<td><select name='" + p.name + "priority'>"
      text += "<option value=0>0</option>"
      text += "<option value=1>1</option>"
      text += "<option value=2>2</option>"
      text += "<option value=3 selected>3</option>"
      text += "<option value=4>4</option>"
      text += "<option value=5>5</option>"
      text += "</select></td></tr>"
    text += "</table>"
    text += "<input type='submit' value='Create Event'>"
    text += "</form>"
    self.response.out.write(text)
    
class CreateEventWithFriends(webapp2.RequestHandler):    #the thing that actually recommends the times
  def post(self):
    text = ''        #for debugging purposes
    friendsInvited = ''  #pass this as a big string to the html, which will pass it to the JavaScript, which will pass it back here
    user = users.get_current_user()
    if not user:
      self.redirect(users.create_login_url(self.request.uri))
    current_k = db.Key.from_path('User', user.nickname())  #get the current user's key
    current = db.get(current_k)          #get the db entity corresponding to the current user
    thekey = current.key()              #get the current user's key...possibly equivalent to two lines up
    q = Friend.all()                      #q is now all Friend entities
    q.ancestor(thekey)                    #q is now filtered to only be Friend entities of the current user
    eName = self.request.get('eventName')  #get all the passed-in values
    hours = self.request.get('hours')
    minutes = self.request.get('minutes')
    earliestStart = self.request.get('earliestStart')
    latestStart = self.request.get('latestStart')
    first = self.request.get('first')
    last = self.request.get('last')
    #invited = list()
    #priority = list()
    #name = list()
    #uMap = list()        #a list of 'unavailability maps' for each friend
    helpers = list()
    #index = 0
    #index2 = 0
    invitedCount = 0
    stupidDate = verifyDateIsNotStupid(first)
    stupidDate2 = verifyDateIsNotStupid(last)
    stupidStart = verifyTimeIsNotStupid(earliestStart)
    stupidEnd = verifyTimeIsNotStupid(latestStart)
    if (stupidDate == "stupid" or stupidDate2 == "stupid"):
      thing = "Date format improper. Proper format for dates is: YYYY-MM-DD"
      template_values = {'message': thing, 'indexNumber1': 1}
      self.response.out.write(template.render('HTML/newIndex.html', template_values))
    elif (stupidStart == "stupid" or stupidEnd == "stupid"):
      thing = "Time format improper. Proper format for times is: HH:MM"
      template_values = {'message': thing, 'indexNumber1': 1}
      self.response.out.write(template.render('HTML/newIndex.html', template_values))
    else:
      for p in q:
        #name.append(p.name)  #for each friend, their name, invitation status, and priority should be at matching indexes
        #invited.append(self.request.get(p.name))
        #priority.append(self.request.get(p.name +'priority'))
        #index += 1  #a count of the total number of friends
        if (self.request.get(p.name) == 'yes'):    #if the friend was invited
          helpers.append(EWFHelper(p.name, self.request.get(p.name), self.request.get(p.name+'priority')))
          if (invitedCount > 0):  #if not the first friend we're adding to the string
            friendsInvited += '?'    #put a question mark in front of the name
          friendsInvited += p.name  #add the name to the string
          invitedCount += 1
  
      if (not eName or not earliestStart or not latestStart or not first or not last):  #if something was left null
        thing = 'Event Incomplete. Event Not Created.'
        template_values = {'message': thing, 'indexNumber1': 1}
        self.response.out.write(template.render('HTML/newIndex.html', template_values))
      elif (invitedCount == 0):  #if no friends were invited
        thing = 'Must Invite At Least 1 Friend'
        template_values = {'message': thing, 'indexNumber1': 1}
        self.response.out.write(template.render('HTML/newIndex.html', template_values))
      elif ("'" in eName):    #if there's an apostrophe in the event name
        thing = "Event name must not contain an apostrophe"
        template_values = {'message': thing, 'indexNumber1' : 1}
        self.response.out.write(template.render('HTML/newIndex.html', template_values))
      elif (">" in eName):
        thing = "Event name must not contain a greater than sign"
        template_values = {'message': thing, 'indexNumber1' : 1}
        self.response.out.write(template.render('HTML/newIndex.html', template_values))
      else:  #carry on
        totalIncrements = list()
        increments = list()
        validStarts = list()
        incrementCount = 0
        validStartsCount = 0
        span = daysBetween(first, last)
        date = first
        earliestInMinutes = toMinutes(earliestStart)
        latestInMinutes = toMinutes(latestStart)
        herp = int(hours)
        derp = int(minutes)
        durationInMinutes = ((herp*60)+(derp))
        start = earliestInMinutes
        end = (latestInMinutes + durationInMinutes)    #end is latest start + duration, all in minutes
        if (end > 1440):    #if the event extends into the next day
          thing = 'Events Can Not Extend Beyond Midnight'
          template_values = {'message': thing, 'indexNumber1': 1}
          self.response.out.write(template.render('HTML/newIndex.html', template_values))
        elif (span == -1):  #if the start and end dates are flipped (or over 100 years apart)
          thing = 'End Date Must Come After Start Date'
          template_values = {'message': thing, 'indexNumber1': 1}
          self.response.out.write(template.render('HTML/newIndex.html', template_values))
        elif (earliestInMinutes >= latestInMinutes):
          thing = 'Latest Start must be after Earliest Start'
          template_values = {'message': thing, 'indexNumber1': 1}
          self.response.out.write(template.render('HTML/newIndex.html', template_values))
        elif (not(earliestInMinutes % 5 == 0) or not(latestInMinutes % 5 == 0)):  #start times are not multiples of 5
          thing = 'Start and End Times must end in multiples of 5 minutes'
          template_values = {'message': thing, 'indexNumber1': 1}
          self.response.out.write(template.render('HTML/newIndex.html', template_values))
        else:  #carry on
          k = 0
          
          #this loop gets ALL increments and loads them into the totalIncrements list
          while (k <= span):    #for each day in the span
            time = start        #time is earliest start, in minutes
            while (time <= end):    #between the earliest possible start and latest possible end
              time2 = backToString(time)    #turn the minutes back into a date string
              totalIncrements.append(Increment(date, time2))  #add an increment corresponding to this date and time
              time += 15        #next increment will be 15 minutes later
              incrementCount += 1
            date = incrementDay(date)    #go to next day
            k += 1
            
          #this loop initializes each invited friend's availabilities
          for f in helpers:    #for each invited friend
            for inc in totalIncrements:  #for each increment
              f.availabilities[inc.date+inc.time] = 1.0  #initialize its availability to 1
          
          #this loop reduces each invited friend's availabilities based on their events
          for f in helpers:    #for each invited friend
            r_k = db.Key.from_path('User', f.name)
            curr = db.get(r_k)
            thekey2 = curr.key()
            r = Event.all()
            r.ancestor(thekey2)    #r is now all of the friend's events
            for s in r:            #for each of the invited friend's events
              for inc in totalIncrements:  #for each increment
                if incrementWithinEvent(inc, s) == 'yes':    #if the increment falls within the event
                  f.availabilities[inc.date+inc.time] -= (float(s.attendance) * .01)  #reduce inc's availability by event's attendance
                  if f.availabilities[inc.date+inc.time] < 0:    #if availability is now negative
                    f.availabilities[inc.date+inc.time] = 0      #just set it to zero
          i = 0
          date = first
          
          #this loop puts all possible start times into the validStarts list
          while (i <= span):    #for each day in the span
            time = start        #time is earliest start, in minutes
            while (time <= latestInMinutes):    #between the earliest possible start and latest possible start
              time2 = backToString(time)    #turn the minutes back into a date string
              validStarts.append(Increment(date, time2))  #add an increment corresponding to this date and time
              time += 15        #next increment will be 15 minutes later
              validStartsCount += 1
            date = incrementDay(date)    #go to next day
            i += 1
          
          options = list()
          #this is the ultimate loop that finally puts it all together
          for w in validStarts:      #for each possible start time
            increments = incrementsForStart(w, durationInMinutes)  #get all increments for the event for the start time
            #text += 'Start: ' + w.date + ' ' + w.time + ' '
            oScore = 0
            moScore = 0  #maximum overall score
            for f in helpers:  #for each invited friend
              aScore = 0
              maScore = 0  #maximum availability score
              for inc in increments:    #for each increment
                iScore = f.availabilities[inc.date+inc.time]  #get the friend's availability at this increment
                aScore += iScore    #add it to the friend's overall availability score
                maScore += 1        #full availability
              pScore = aScore * int(f.priority)  #scale overall availability score by friend's priority
              mpScore = maScore * int(f.priority)
              oScore += pScore    #add pScore to start time's overall score
              moScore += mpScore
            percentScore = 100*(oScore/moScore)
            wmin = toMinutes(w.time)    #w's start time
            finishedmin = wmin + durationInMinutes  #w's end time, in minutes
            finished = backToString(finishedmin)    #w's end time, as a string
            options.append(Option(w.date, w.time, finished, percentScore)) #add an option for start w with its calculated overall score
          
          options.sort(key=lambda x: x.score, reverse=True)
          #for o in options:
            #text += 'Option: ' + o.date + ' ' + o.time + ' ' + str(o.score) + ' '
          template_values = {'message' : text, 'options' : options, 'indexNumber1' : 1, 'sender' : user.nickname(), 'friendsinvited' : friendsInvited, 'ename': eName}
          self.response.out.write(template.render('HTML/newIndex.html', template_values))  #give back the main page again
        
class AjaxEnvitations(webapp2.RequestHandler):    #displays all Envitations
  def post(self):
    text = ""
    user = users.get_current_user()
    current_k = db.Key.from_path('User', user.nickname())  #get the current user's key
    current = db.get(current_k)          #get the db entity corresponding to the current user
    thekey = current.key()              #get the current user's key...possibly equivalent to two lines up
    #q = memcache.get(thekey)				#get q from the memcache
    #if q is None:
    q = Envitation.all()                      #q is now all Envitation entities
    q.ancestor(thekey)                    #q is now filtered to only be Envitation entities of the current user
    #memcache.add(thekey,q)				#add q to the memcache under the key thekey
    count = 0
    for p in q:
    #for each Envitation
      count += 1
      text += ('<p>From: ' + p.sender + '</p>')
  #show its information
      text += '<p>Event: ' + p.eName + '</p>'
      text += '<p>Date: ' + p.date + '</p>'
      text += '<p>Start: ' + p.start + '</p>'
      text += '<p>End: ' + p.end + '</p>'
      key2 = str(p.key())
      text += '<button onclick="ajaxEAccept('
    #an accept button
      text += '\'' + key2 + '\','
      text += '\'' + p.eName + '\','
      text += '\'' + p.date + '\','
      text += '\'' + p.start + '\','
      text += '\'' + p.end + '\''
      text += ')">Accept</button>'
      text += '<button onclick="ajaxEIgnore('
    #an ignore button
      text += '\'' + key2 + '\''
      text += ')">Ignore</button>'
      text += '<hr>'    #a division thing
    if (count == 0):  #if no Envitations
      text += "You don't have any Event invitations"
    self.response.out.write(text)
    
class AjaxEIgnore(webapp2.RequestHandler):    #when the ignore button is hit
  def post(self):
    thekey = self.request.get('thekey')
    db.delete(thekey)
    self.response.out.write('Invitation Ignored')
    
class AjaxEAccept(webapp2.RequestHandler):    #when the accept button is hit
  def post(self):
    key2 = self.request.get('thekey') 
    eName = self.request.get('eName')
    date = self.request.get('date')
    start = self.request.get('start')
    end = self.request.get('end')
    text = ''
    text += '<p>Attendance Probability</p>'
    text += '<input type="range" id="attendanceProbability">'  #allows attendance probability to be specified
    text += '<button onclick="ajaxEAccept2('   #button that actually adds the event to the user's events 
    text += '\'' + key2 + '\','
    text += '\'' + eName + '\','
    text += '\'' + date + '\','
    text += '\'' + start + '\','
    text += 'document.getElementById(\'attendanceProbability\').value,'
    text += '\'' + end + '\''
    text += ')">Confirm</button>'
    self.response.out.write(text)
    
class AjaxEAccept2(webapp2.RequestHandler):  #this is where the event is actually created for an accepted invitation
  def post(self):
    key2 = self.request.get('thekey')
    eName = self.request.get('eName')
    date = self.request.get('date')
    start = self.request.get('start')
    end = self.request.get('end')
    probability = self.request.get('probability')
    stupidProbability = verifyAttendanceIsNotStupid(probability)
    if (stupidProbability == "stupid"):
      text = 'Attendance format improper. Attendance must be an integer 0-100.'
      self.response.out.write(text)
    else:
      user = users.get_current_user()                #get the current user
      current_k = db.Key.from_path('User', user.nickname())  #get the current user's key
      current = db.get(current_k)          #get the db entity corresponding to the current user
      thekey = current.key()              #get the current user's key...possibly equivalent to two lines up
      event = Event(parent=thekey)        #so that this event will be a child of the current user
      event.name = eName                    #set the event's properties
      event.date = date
      event.start = start
      event.end = end
      event.attendance = probability
      event.put()        #put the event into the datastore
      db.delete(key2)    #delete the Envitation, because it's just been accepted
      text = 'Invitation Accepted. Event Added.'
      self.response.out.write(text)
    
class AjaxSendEnvitations(webapp2.RequestHandler):  #sends invitations to invited friends for a start time
  def post(self):
    text = ''
    sender = self.request.get("sender")
    eName = self.request.get("eName")
    date = self.request.get("date")
    time = self.request.get("time")
    finished = self.request.get("finished")
    friendsInvited = self.request.get("friendsInvited")
    friends = friendsInvited.split("?")
    #text += 'Sender: ' + sender + ' Date: ' + date + ' Time: ' + time + ' Finished: ' + finished + ' Name: ' + eName
    #text += ' Friends Invited: '
    for f in friends:    #for each friend invited
      recipient_k = db.Key.from_path('User', f)  #the User entity of friend f
      curr = Envitation(parent=recipient_k)  #so that this Envitation will be a child of the recipient
      curr.sender = sender  #set the Envitation's values
      curr.date = date
      curr.start = time
      curr.end = finished
      curr.eName = eName
      curr.put()          #add it to the database
    user = users.get_current_user()                #get the current user
    current_k = db.Key.from_path('User', user.nickname())  #get the current user's key
    current = db.get(current_k)          #get the db entity corresponding to the current user
    thekey = current.key()              #get the current user's key...possibly equivalent to two lines up
    event = Event(parent=thekey)        #create an Event for the invitation sender
    event.name = eName                    #set the event's properties
    event.date = date
    event.start = time
    event.end = finished
    event.attendance = '75'            #75 seems reasonable; it says 'probably', but not 'definitely'
    event.put()                        #add the event
    self.response.out.write('Invitation(s) sent')
    
def incrementWithinEvent(increment, event):  #does the increment overlap with the event?
  incDate = increment.date
  eDate = event.date
  time = increment.time
  eStart = event.start
  eEnd = event.end
  sameDate = 'no'
  if (incDate == eDate):  #if the date of the increment is the same as the date of the event
    sameDate = 'yes'      
  sameTime = timeWithinEvent(time, event)  #function determines whether the increment time falls within event's start and end
  if (sameDate == "yes" and sameTime == "yes"):
    return "yes"
  occursOnDate = eventOccursOnDate(event, incDate)
  if (occursOnDate == "yes" and sameTime == "yes"):  #event occurs on the increment date, and the times overlap
    return "yes"
  return 'no'  #the increment and the event do not overlap
  
    
def incrementsForStart(startIncrement, duration):    #returns the increments for the event, given its start increment and duration
  start = toMinutes(startIncrement.time)
  end = start + duration
  time = start
  increments = list()
  while (time <= end):
    time2 = backToString(time)
    increments.append(Increment(startIncrement.date, time2))
    time += 15
  return increments
    
def timeWithinEvent(time, event):  #is the time within the start and end times of event?
  result = timeWithin(time, event.start, event.end)
  return result
    
def timeWithin(timeC, timeA, timeB):  #is timeC within the range of timeA and timeB? (with timeB as the later time)
  hoursA = int(getHours(timeA))
  hoursB = int(getHours(timeB))  #get the individual components of each time
  hoursC = int(getHours(timeC))
  minutesA = int(getMinutes(timeA))
  minutesB = int(getMinutes(timeB))
  minutesC = int(getMinutes(timeC))
  totalA = ((hoursA*60) + minutesA)  #convert the times into minutes
  totalB = ((hoursB*60) + minutesB)
  totalC = ((hoursC*60) + minutesC)
  if ((totalC >= totalA) and (totalC <= totalB)):  #if timeC's minutes are at least timeA's and no more than timeB's
    return 'yes'
  else:
    return 'no'
  
def backToString(minutesTotal):
  hours = minutesTotal/60
  minutes = minutesTotal%60
  return (str(hours) + ':' + str(minutes))
    
  
def getHours(time):      #returns the hour component of a time (as a string)
  pieces = time.split(':')
  return pieces[0]

def getMinutes(time):    #returns the minute component of a time (as a string)
  pieces = time.split(':')
  return pieces[1]
 
def toMinutes(time):        #converts a time to total minutes
  hours = int(getHours(time))
  minutes = int(getMinutes(time))
  return ((hours*60)+minutes)  

def daysBetween(first, last):  #returns the number of days between two dates
  counter = 0
  while (not(first == last)):    #until the dates are equal
    first = incrementDay(first)  #increment the day
    counter += 1                  #increment the counter
    if counter > 36500:        #if they're more than 100 years apart
      return -1                #assume that last is before first, return -1
  return counter

def noZeroes(date):
  month = int(getMonth(date))  #convert to ints, removing leading zeroes
  year = int(getYear(date))
  day = int(getDay(date))
  return (str(year)+'-'+str(month)+'-'+str(day))  #deconvert back to string and return

def incrementDay(first):      #returns the day after the date given as the argument
  month = int(getMonth(first))
  year = int(getYear(first))
  day = int(getDay(first))
  if (day < 28):      #if less than 28 days into the month
    day += 1          #just increment the day
    if (day < 10 and month < 10):
      return (str(year)+'-0'+str(month)+'-0'+str(day))  #return new date as a string
    elif (day < 10):
      return (str(year)+'-'+str(month)+'-0'+str(day))  #return new date as a string
    elif (month < 10):
      return (str(year)+'-0'+str(month)+'-'+str(day))  #return new date as a string
    return (str(year)+'-'+str(month)+'-'+str(day))  #return new date as a string
  elif (day == 28 and month == 2):  #if it's February 28
    leap = determineIfLeapYear(year)  #function to check if it's a leap year
    if leap == 'yes':  #if it's a leap year
      day += 1    #just increment the day
      if (day < 10 and month < 10):
        return (str(year)+'-0'+str(month)+'-0'+str(day))  #return new date as a string
      elif (day < 10):
        return (str(year)+'-'+str(month)+'-0'+str(day))  #return new date as a string
      elif (month < 10):
        return (str(year)+'-0'+str(month)+'-'+str(day))  
      return (str(year)+'-'+str(month)+'-'+str(day))  
    else:  #if it's not a leap year
      month = 3  #set the date to March 1
      day = 1
      if (day < 10 and month < 10):
        return (str(year)+'-0'+str(month)+'-0'+str(day)) 
      elif (day < 10):
        return (str(year)+'-'+str(month)+'-0'+str(day)) 
      elif (month < 10):
        return (str(year)+'-0'+str(month)+'-'+str(day)) 
      return (str(year)+'-'+str(month)+'-'+str(day))  #return new date as a string
  elif (day == 28):  #it's the 28th of some other month
    day += 1
    if (day < 10 and month < 10):
      return (str(year)+'-0'+str(month)+'-0'+str(day))
    elif (day < 10):
      return (str(year)+'-'+str(month)+'-0'+str(day))  
    elif (month < 10):
      return (str(year)+'-0'+str(month)+'-'+str(day)) 
    return (str(year)+'-'+str(month)+'-'+str(day))  #return new date as a string
  elif (day == 29 and month == 2):  #it's February 29, so must have been a leap year
    month = 3  #set the date to March 1
    day = 1
    if (day < 10 and month < 10):
      return (str(year)+'-0'+str(month)+'-0'+str(day))
    elif (day < 10):
      return (str(year)+'-'+str(month)+'-0'+str(day))
    elif (month < 10):
      return (str(year)+'-0'+str(month)+'-'+str(day))
    return (str(year)+'-'+str(month)+'-'+str(day))  #return new date as a string
  elif (day == 29):  #it's the 29th of some other month
    day += 1
    if (day < 10 and month < 10):
      return (str(year)+'-0'+str(month)+'-0'+str(day))
    elif (day < 10):
      return (str(year)+'-'+str(month)+'-0'+str(day))
    elif (month < 10):
      return (str(year)+'-0'+str(month)+'-'+str(day))
    return (str(year)+'-'+str(month)+'-'+str(day))  #return new date as a string
  elif (day == 30 and (month == 9 or month == 4 or month == 6 or month == 11)):  #30th of September, April, June, or November
    day = 1    #new date is the first of the next month
    month += 1
    if (day < 10 and month < 10):
      return (str(year)+'-0'+str(month)+'-0'+str(day))
    elif (day < 10):
      return (str(year)+'-'+str(month)+'-0'+str(day))
    elif (month < 10):
      return (str(year)+'-0'+str(month)+'-'+str(day))
    return (str(year)+'-'+str(month)+'-'+str(day))  #return new date as a string
  elif (day == 30):  #30th of some other month
    day += 1  #just increment the day
    if (day < 10 and month < 10):
      return (str(year)+'-0'+str(month)+'-0'+str(day))
    elif (day < 10):
      return (str(year)+'-'+str(month)+'-0'+str(day))
    elif (month < 10):
      return (str(year)+'-0'+str(month)+'-'+str(day))
    return (str(year)+'-'+str(month)+'-'+str(day))  #return new date as a string
  elif (day == 31 and month == 12):  #December 31st
    day = 1  #set date to be January 1 of the next year
    month = 1
    year += 1
    if (day < 10 and month < 10):
      return (str(year)+'-0'+str(month)+'-0'+str(day))
    elif (day < 10):
      return (str(year)+'-'+str(month)+'-0'+str(day))
    elif (month < 10):
      return (str(year)+'-0'+str(month)+'-'+str(day))
    return (str(year)+'-'+str(month)+'-'+str(day))  #return new date as a string
  else:  #31st of some other month
    day = 1
    month += 1  #new date is the first of the next month
    if (day < 10 and month < 10):
      return (str(year)+'-0'+str(month)+'-0'+str(day))
    elif (day < 10):
      return (str(year)+'-'+str(month)+'-0'+str(day))
    elif (month < 10):
      return (str(year)+'-0'+str(month)+'-'+str(day))
    return (str(year)+'-'+str(month)+'-'+str(day))  #return new date as a string

def determineIfLeapYear(year):
  leap = 'no'
  if (year%4 == 0):  #if it's a fourth year
    leap = 'yes'
  if (year%100 == 0):  #if it's a hundredth year
    leap = 'no'
  if (year%400 == 0):  #if it's a four hundredth year
    leap = 'yes'
  return leap

def ofWeek(date):          #given a date, returns which day of the week it is (0-6)
  year = int(getYear(date))
  month = int(getMonth(date))
  day = int(getDay(date))
  return weekDay(year, month, day)

def getYear(date):
  pieces = date.split('-')
  return pieces[0]

def getMonth(date):
  pieces = date.split('-')
  return pieces[1]

def getDay(date):
  pieces = date.split('-')
  return pieces[2]

def weekDay(year, month, day):
    offset = [0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334]
    afterFeb = 1
    if month > 2: afterFeb = 0
    aux = year - 1700 - afterFeb
    # dayOfWeek for 1700/1/1 = 5, Friday
    dayOfWeek  = 5
    # partial sum of days between current date and 1700/1/1
    dayOfWeek += (aux + afterFeb) * 365                  
    # leap year correction    
    dayOfWeek += aux / 4 - aux / 100 + (aux + 100) / 400     
    # sum monthly and day offsets
    dayOfWeek += offset[month - 1] + (day - 1)               
    dayOfWeek %= 7
    return dayOfWeek 
    
class DoDumbTest(webapp2.RequestHandler):
  def post(self):
    self.response.out.write('Hi')
    
class GetUserEventList(webapp2.RequestHandler):
  def post(self):
    text = ''
    user = users.get_current_user()
    current_k = db.Key.from_path('User', user.nickname())  #get the current user's key
    current = db.get(current_k)          #get the db entity corresponding to the current user
    thekey = current.key()              #get the current user's key...possibly equivalent to two lines up
    q = Event.all()                      #q is now all Event entities
    q.ancestor(thekey)
                    #q is now Event entities of the current user
    text += '{'
    text += '"list": ['
    for p in q:
      time = timeExpandedForm(p.start)  #adds seconds to the time, always assumed to be 00
      text += '{'                #beginning of the event
      text += '"title": '
      text += '"' + p.name + '"'
      text += ', "start": '
      text += '"' + p.date + '"' 
      text += '}'                #end of the event
      text += ','                #comma separating events
      if (p.sun):            #if the event repeats on sundays
        firstSpecifiedDay = getFirstSpecifiedDay(p.date, 0)  #get the first, for instance, sunday on or after the specified date
        if (not (firstSpecifiedDay == p.date)):    #if the date was not that day
          text += '{'                #beginning of the event
          text += '"title": '
          text += '"' + p.name + '"'
          text += ', "start": '
          text += '"' + firstSpecifiedDay + '"' 
  #add an event for that day
          text += '}'                #end of the event
          text += ','                #comma separating events
        counter = 0
        day = incrementWeek(firstSpecifiedDay)  #advance a week
        while (counter < 13):    #for half a year
          text += '{'                #beginning of the event
          text += '"title": '
          text += '"' + p.name + '"'
          text += ', "start": '
          text += '"' + day + '"' 
    #add an event for that day
          text += '}'                #end of the event
          text += ','                #comma separating events
          day = incrementWeek(day)  #advance a week
          counter += 1
      if (p.mon):            #if the event repeats on mondays
        firstSpecifiedDay = getFirstSpecifiedDay(p.date, 1)  #get the first, for instance, sunday on or after the specified date
        if (not (firstSpecifiedDay == p.date)):    #if the date was not that day
          text += '{'                #beginning of the event
          text += '"title": '
          text += '"' + p.name + '"'
          text += ', "start": '
          text += '"' + firstSpecifiedDay + '"' 
  #add an event for that day
          text += '}'                #end of the event
          text += ','                #comma separating events
        counter = 0
        day = incrementWeek(firstSpecifiedDay)  #advance a week
        while (counter < 13):    #for half a year
          text += '{'                #beginning of the event
          text += '"title": '
          text += '"' + p.name + '"'
          text += ', "start": '
          text += '"' + day + '"' 
    #add an event for that day
          text += '}'                #end of the event
          text += ','                #comma separating events
          day = incrementWeek(day)  #advance a week
          counter += 1   
      if (p.tues):            #if the event repeats on tuesdays
        firstSpecifiedDay = getFirstSpecifiedDay(p.date, 2)  #get the first, for instance, sunday on or after the specified date
        if (not (firstSpecifiedDay == p.date)):    #if the date was not that day
          text += '{'                #beginning of the event
          text += '"title": '
          text += '"' + p.name + '"'
          text += ', "start": '
          text += '"' + firstSpecifiedDay + '"' 
  #add an event for that day
          text += '}'                #end of the event
          text += ','                #comma separating events
        counter = 0
        day = incrementWeek(firstSpecifiedDay)  #advance a week
        while (counter < 13):    #for half a year
          text += '{'                #beginning of the event
          text += '"title": '
          text += '"' + p.name + '"'
          text += ', "start": '
          text += '"' + day + '"' 
    #add an event for that day
          text += '}'                #end of the event
          text += ','                #comma separating events
          day = incrementWeek(day)  #advance a week
          counter += 1 
      if (p.wed):            #if the event repeats on wednesdays
        firstSpecifiedDay = getFirstSpecifiedDay(p.date, 3)  #get the first, for instance, sunday on or after the specified date
        if (not (firstSpecifiedDay == p.date)):    #if the date was not that day
          text += '{'                #beginning of the event
          text += '"title": '
          text += '"' + p.name + '"'
          text += ', "start": '
          text += '"' + firstSpecifiedDay + '"' 
  #add an event for that day
          text += '}'                #end of the event
          text += ','                #comma separating events
        counter = 0
        day = incrementWeek(firstSpecifiedDay)  #advance a week
        while (counter < 13):    #for half a year
          text += '{'                #beginning of the event
          text += '"title": '
          text += '"' + p.name + '"'
          text += ', "start": '
          text += '"' + day + '"' 
    #add an event for that day
          text += '}'                #end of the event
          text += ','                #comma separating events
          day = incrementWeek(day)  #advance a week
          counter += 1   
      if (p.thurs):            #if the event repeats on thursdays
        firstSpecifiedDay = getFirstSpecifiedDay(p.date, 4)  #get the first, for instance, sunday on or after the specified date
        if (not (firstSpecifiedDay == p.date)):    #if the date was not that day
          text += '{'                #beginning of the event
          text += '"title": '
          text += '"' + p.name + '"'
          text += ', "start": '
          text += '"' + firstSpecifiedDay + '"' 
  #add an event for that day
          text += '}'                #end of the event
          text += ','                #comma separating events
        counter = 0
        day = incrementWeek(firstSpecifiedDay)  #advance a week
        while (counter < 13):    #for half a year
          text += '{'                #beginning of the event
          text += '"title": '
          text += '"' + p.name + '"'
          text += ', "start": '
          text += '"' + day + '"' 
    #add an event for that day
          text += '}'                #end of the event
          text += ','                #comma separating events
          day = incrementWeek(day)  #advance a week
          counter += 1 
      if (p.fri):            #if the event repeats on fridays
        firstSpecifiedDay = getFirstSpecifiedDay(p.date, 5)  #get the first, for instance, sunday on or after the specified date
        if (not (firstSpecifiedDay == p.date)):    #if the date was not that day
          text += '{'                #beginning of the event
          text += '"title": '
          text += '"' + p.name + '"'
          text += ', "start": '
          text += '"' + firstSpecifiedDay + '"' 
  #add an event for that day
          text += '}'                #end of the event
          text += ','                #comma separating events
        counter = 0
        day = incrementWeek(firstSpecifiedDay)  #advance a week
        while (counter < 13):    #for half a year
          text += '{'                #beginning of the event
          text += '"title": '
          text += '"' + p.name + '"'
          text += ', "start": '
          text += '"' + day + '"' 
    #add an event for that day
          text += '}'                #end of the event
          text += ','                #comma separating events
          day = incrementWeek(day)  #advance a week
          counter += 1
    if (p.sat):            #if the event repeats on saturdays
        firstSpecifiedDay = getFirstSpecifiedDay(p.date, 6)  #get the first, for instance, sunday on or after the specified date
        if (not (firstSpecifiedDay == p.date)):    #if the date was not that day
          text += '{'                #beginning of the event
          text += '"title": '
          text += '"' + p.name + '"'
          text += ', "start": '
          text += '"' + firstSpecifiedDay + '"' 
  #add an event for that day
          text += '}'                #end of the event
          text += ','                #comma separating events
        counter = 0
        day = incrementWeek(firstSpecifiedDay)  #advance a week
        while (counter < 13):    #for a quarter of a year
          text += '{'                #beginning of the event
          text += '"title": '
          text += '"' + p.name + '"'
          text += ', "start": '
          text += '"' + day + '"' 
    #add an event for that day
          text += '}'                #end of the event
          text += ','                #comma separating events
          day = incrementWeek(day)  #advance a week
          counter += 1
    text = text[:-1]            #should remove that last comma
    text += ']'
    text += '}'
    self.response.out.write(text)
    
def timeExpandedForm(time):
  time = time + ":00"
  return time
    
def getFirstSpecifiedDay(date, day):  #gets the first 'day' after 'date', where 'day' is a day of the week 0-6
  dayOfWeek = ofWeek(date)  #gets date's day of the week
  while (not (dayOfWeek == day)):  #until the date is the correct day of the week
    date = incrementDay(date)    #advance a day
    dayOfWeek = ofWeek(date)     #update its day of the week
  return date

def incrementWeek(day):          #given a date as a string, returns the date a week later as a string
  count = 0
  while (count < 7):    #seven times
    day = incrementDay(day)  #increment the day
    count += 1
  return day

def verifyDateIsNotStupid(date):
  pieces = date.split('-')
  length = len(pieces)
  if (not length == 3):      #if there are not exactly 3 components after splitting
    return "stupid"          #the date is stupid
  if (not pieces[0].isdigit() or not pieces[1].isdigit() or not pieces[2].isdigit()):  #if not all pieces are numbers
    return "stupid"          #the date is stupid
  year = int(pieces[0])      #convert to integers
  month = int(pieces[1])
  day = int(pieces[2])
  if (day > 31):            #if the day is over 31
    return "stupid"          #the date is stupid
  if (month > 12):      #if the month is over 12
    return "stupid"    #the date is stupid
  if (year > 4000):  #if the year is over 4000
    return "stupid"  #the date is stupid
  if (day > 30 and (month == 2 or month == 4 or month == 6 or month == 9 or month == 11)): #days over 30 and a month with < 31 days
    return "stupid"  #the date is stupid
  leap = determineIfLeapYear(year)  #determines if the year is a leap year
  if (leap == "yes" and month == 2 and day > 29):  #if Feb 30 or more on a leap year
    return "stupid"  #the date is stupid
  if (leap == "no" and month == 2 and day > 28):  #if Feb 29 or more on a non leap year
    return "stupid"  #the date is stupid
  return "not stupid"    #if we've made it this far, the date is not stupid
  
def verifyTimeIsNotStupid(time):
  pieces = time.split(':')
  length = len(pieces)
  if (not length == 2):    #if there are not exactly 2 components after splitting
    return "stupid"        #the time is stupid
  if (not pieces[0].isdigit() or not pieces[1].isdigit()):  #if either component is not a number
    return "stupid"        #the time is stupid
  hours = int(pieces[0])
  minutes = int(pieces[1])
  if (hours > 23):        #if hours is more than 23
    return "stupid"      #the time is stupid
  if (minutes > 59):    #if minutes is more than 59
    return "stupid"    #the time is stupid
  return "not stupid"   #if we've made it this far, the time is not stupid
  
def verifyAttendanceIsNotStupid(attendance):
  if (not attendance.isdigit()):    #if the attendance is not a number
    return "stupid"  #the attendance is stupid
  number = int(attendance)
  if (number > 100):      #if the attendance is over 100
    return "stupid"    #the attendance is stupid
  return "not stupid"  #if we've made it this far, the attendance is not stupid

def eventOccursOnDate(event, date):
  rSun = 'no'
  rMon = 'no'
  rTues = 'no'
  rWed = 'no'
  rThurs = 'no'
  rFri = 'no'
  rSat = 'no'
  if event.sun:
    rSun = 'yes'
  if event.mon:
    rMon = 'yes'
  if event.tues:
    rTues = 'yes'
  if event.wed:
    rWed = 'yes'
  if event.thurs:
    rThurs = 'yes'
  if event.fri:
    rFri = 'yes'
  if event.sat:
    rSat = 'yes'
  eDate = event.date
  if (eDate == date):    #if the event date and the specified date match
    return "yes"
  if (rSun == "yes"):    #if the event repeats on Sundays
    first = getFirstSpecifiedDay(eDate, 0)  #get the first sunday on or after the event date
    if (first == date):    #if it matches the specified date
      return "yes"
    counter = 0
    day = incrementWeek(first)  #advance a week
    while (counter < 104):    #for two years
      if (day == date):      #if that day matches the specified date
        return "yes"
      day = incrementWeek(day)  #advance a week
      counter += 1
  if (rMon == "yes"):    #if the event repeats on Sundays
    first = getFirstSpecifiedDay(eDate, 1)  #get the first monday on or after the event date
    if (first == date):    #if it matches the specified date
      return "yes"
    counter = 0
    day = incrementWeek(first)  #advance a week
    while (counter < 104):    #for two years
      if (day == date):      #if that day matches the specified date
        return "yes"
      day = incrementWeek(day)  #advance a week
      counter += 1
  if (rTues == "yes"):    #if the event repeats on Sundays
    first = getFirstSpecifiedDay(eDate, 2)  #get the first tuesday on or after the event date
    if (first == date):    #if it matches the specified date
      return "yes"
    counter = 0
    day = incrementWeek(first)  #advance a week
    while (counter < 104):    #for two years
      if (day == date):      #if that day matches the specified date
        return "yes"
      day = incrementWeek(day)  #advance a week
      counter += 1
  if (rWed == "yes"):    #if the event repeats on Sundays
    first = getFirstSpecifiedDay(eDate, 3)  #get the first wednesday on or after the event date
    if (first == date):    #if it matches the specified date
      return "yes"
    counter = 0
    day = incrementWeek(first)  #advance a week
    while (counter < 104):    #for two years
      if (day == date):      #if that day matches the specified date
        return "yes"
      day = incrementWeek(day)  #advance a week
      counter += 1
  if (rThurs == "yes"):    #if the event repeats on Sundays
    first = getFirstSpecifiedDay(eDate, 4)  #get the first thursday on or after the event date
    if (first == date):    #if it matches the specified date
      return "yes"
    counter = 0
    day = incrementWeek(first)  #advance a week
    while (counter < 104):    #for two years
      if (day == date):      #if that day matches the specified date
        return "yes"
      day = incrementWeek(day)  #advance a week
      counter += 1
  if (rFri == "yes"):    #if the event repeats on Sundays
    first = getFirstSpecifiedDay(eDate, 5)  #get the first friday on or after the event date
    if (first == date):    #if it matches the specified date
      return "yes"
    counter = 0
    day = incrementWeek(first)  #advance a week
    while (counter < 104):    #for two years
      if (day == date):      #if that day matches the specified date
        return "yes"
      day = incrementWeek(day)  #advance a week
      counter += 1
  if (rSat == "yes"):    #if the event repeats on Sunday
    first = getFirstSpecifiedDay(eDate, 6)  #get the first saturday on or after the event date
    if (first == date):    #if it matches the specified date
      return "yes"
    counter = 0
    day = incrementWeek(first)  #advance a week
    while (counter < 104):    #for two years
      if (day == date):      #if that day matches the specified date
        return "yes"
      day = incrementWeek(day)  #advance a week
      counter += 1
  return "no"  #after all that we never found a match, return no
      
    
#maps handlers to actions    
app = webapp2.WSGIApplication([
  ('/', MainPage),
  ('/login', LogIn),
  ('/logout', LogOut)
,
  ('/createEvent', CreateEvent),
  ('/manageEvents', ManageEvents),
  ('/deleteEvent', DeleteEvent)
,
  ('/friends', Friends),
  ('/frinvitationSend', FrinvitationSend),
  ('/displayFrinvitations', DisplayFrinvitations),
  ('/accept', Accept),
  ('/ignore', Ignore),
  ('/unfriend', Unfriend)
,
  ('/ajaxManageEvents', AjaxManageEvents),
  ('/ajaxDeleteEvent', AjaxDeleteEvent),
  ('/ajaxFriends', AjaxFriends),
  ('/ajaxUnfriend', AjaxUnfriend),
  ('/ajaxSendFrinvitation', AjaxSendFrinvitation),
  ('/ajaxFrinvitations', AjaxFrinvitations),
  ('/ajaxAccept', AjaxAccept),
  ('/ajaxIgnore', AjaxIgnore),
  ('/ajaxEventWithFriends', AjaxEventWithFriends),
  ('/createEventWithFriends', CreateEventWithFriends),
  ('/ajaxEnvitations', AjaxEnvitations),
  ('/ajaxEIgnore', AjaxEIgnore),
  ('/ajaxEAccept', AjaxEAccept),
  ('/ajaxSendEnvitations', AjaxSendEnvitations),
  ('/ajaxEAccept2', AjaxEAccept2),
  ('/getUserEventList', GetUserEventList)
], debug=True)