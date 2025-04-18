// app.js - Event Scheduler Frontend Logic

// Store events to be scheduled
let eventsToSchedule = [];

// DOM elements
document.addEventListener('DOMContentLoaded', () => {
    const addEventForm = document.getElementById('add-event-form');
    const eventsList = document.getElementById('events-list');
    const scheduleButton = document.getElementById('schedule-events');
    const clearButton = document.getElementById('clear-all');
    const scheduledEventsList = document.getElementById('scheduled-events-list');
    const errorMessage = document.getElementById('error-message');
    
    // Add event form submission
    if (addEventForm) {
        addEventForm.addEventListener('submit', function(e) {
            e.preventDefault();
            
            // Get form values
            const eventName = document.getElementById('event-name').value;
            const username = document.getElementById('username').value;
            const venue = document.getElementById('venue').value;
            const date = document.getElementById('date').value;
            const startTime = document.getElementById('start-time').value;
            const endTime = document.getElementById('end-time').value;
            const priority = document.getElementById('priority').checked;
            
            // Calculate duration in minutes
            const startMinutes = timeToMinutes(startTime);
            const endMinutes = timeToMinutes(endTime);
            const duration = endMinutes - startMinutes;
            
            // Create event object
            const event = {
                event_name: eventName,
                username: username,
                venue: venue,
                date: date,
                start_time: startTime,
                end_time: endTime,
                priority: priority,
                duration: duration
            };
            
            // Add to events array
            eventsToSchedule.push(event);
            
            // Update UI
            updateEventsList();
            
            // Reset form
            addEventForm.reset();
        });
    }
    
    // Schedule events button
    if (scheduleButton) {
        scheduleButton.addEventListener('click', function() {
            if (eventsToSchedule.length === 0) {
                showError('Please add at least one event to schedule');
                return;
            }
            
            // Clear previous error
            hideError();
            
            // Send to backend
            fetch('/schedule_events/', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify({ events: eventsToSchedule })
            })
            .then(response => {
                if (!response.ok) {
                    throw new Error('Server returned ' + response.status);
                }
                return response.json();
            })
            .then(data => {
                // Display scheduled events
                displayScheduledEvents(data.scheduled_events);
            })
            .catch(error => {
                showError('Failed to fetch: ' + error.message);
                console.error('Error:', error);
            });
        });
    }
    
    // Clear all button
    if (clearButton) {
        clearButton.addEventListener('click', function() {
            eventsToSchedule = [];
            updateEventsList();
            scheduledEventsList.innerHTML = '';
            hideError();
        });
    }
    
    // Helper functions
    function updateEventsList() {
        if (!eventsList) return;
        
        eventsList.innerHTML = '';
        
        // Update event count
        const eventCount = document.getElementById('event-count');
        if (eventCount) {
            eventCount.textContent = eventsToSchedule.length + ' events';
        }
        
        eventsToSchedule.forEach((event, index) => {
            const eventItem = document.createElement('div');
            eventItem.className = 'event-item';
            eventItem.innerHTML = `
                <div class="event-header">${event.event_name}</div>
                <div class="event-details">
                    <p>By: ${event.username} | Venue: ${event.venue}</p>
                    <p>Date: ${formatDate(event.date)}</p>
                    <p>Time: ${event.start_time} - ${event.end_time}</p>
                </div>
            `;
            
            // Add a blue vertical line to the left
            eventItem.style.borderLeft = '4px solid #3498db';
            eventItem.style.paddingLeft = '10px';
            eventItem.style.marginBottom = '15px';
            
            eventsList.appendChild(eventItem);
        });
    }
    
    function displayScheduledEvents(events) {
        if (!scheduledEventsList) return;
        
        scheduledEventsList.innerHTML = '';
        
        events.forEach(event => {
            const eventItem = document.createElement('div');
            eventItem.className = 'scheduled-event-item';
            eventItem.innerHTML = `
                <div class="event-header">${event.event_name}</div>
                <div class="event-details">
                    <p>By: ${event.username} | Venue: ${event.venue}</p>
                    <p>Date: ${formatDate(event.date)}</p>
                    <p>Time: ${event.start_time} - ${event.end_time}</p>
                    <p>Priority: ${event.priority ? 'High' : 'Normal'}</p>
                </div>
            `;
            
            // Add a green vertical line to the left
            eventItem.style.borderLeft = '4px solid #2ecc71';
            eventItem.style.paddingLeft = '10px';
            eventItem.style.marginBottom = '15px';
            
            scheduledEventsList.appendChild(eventItem);
        });
    }
    
    function showError(message) {
        if (!errorMessage) return;
        
        errorMessage.textContent = message;
        errorMessage.style.display = 'block';
    }
    
    function hideError() {
        if (!errorMessage) return;
        
        errorMessage.style.display = 'none';
    }
    
    function timeToMinutes(timeStr) {
        const [hours, minutes] = timeStr.split(':').map(Number);
        return hours * 60 + minutes;
    }
    
    function formatDate(dateStr) {
        const date = new Date(dateStr);
        return date.toLocaleDateString('en-US', { 
            weekday: 'short', 
            year: 'numeric', 
            month: 'short', 
            day: 'numeric' 
        });
    }
});
