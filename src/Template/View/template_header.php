<header class="app-ui__header shadow-xs padding-x-md padding-x-0@md">
    <div class="app-ui__logo-wrapper padding-x-sm@md">
        <a href="https://usadmin1.questblue.com/" class="app-ui__logo">
           <img src="/public/img/qb_logo.svg" height="110" width="200" alt="CheapStream Logo">
        </a>
    </div>
    
    <!-- (mobile-only) menu button -->
    <button class="reset app-ui__menu-btn hide@md js-app-ui__menu-btn js-tab-focus" aria-label="Toggle menu" aria-controls="app-ui-navigation">
        <svg class="icon" viewBox="0 0 24 24">
            <g class="icon__group" fill="none" stroke="currentColor" stroke-linecap="square" stroke-miterlimit="10" stroke-width="2">
                <path d="M1 6h22" />
                <path d="M1 12h22" />
                <path d="M1 18h22" />
            </g>
        </svg>
    </button>
    
    <!-- (desktop-only) header menu -->
    <div class="display@md flex flex-grow height-100% items-center justify-between padding-x-sm">
        <form class="expandable-search text-sm@md js-expandable-search">
            <!-- Search bar if we ever want it
            <label class="sr-only" for="expandable-search">Search</label>
            <input class="reset expandable-search__input js-expandable-search__input" type="search" name="expandable-search" id="expandable-search" placeholder="Search...">
            <button class="reset expandable-search__btn">
                <svg class="icon" viewBox="0 0 20 20">
                    <title>Search</title>
                    <g fill="none" stroke="currentColor" stroke-miterlimit="10" stroke-width="2">
                        <circle cx="8" cy="8" r="6" />
                        <line x1="12.243" y1="12.243" x2="18" y2="18" />
                    </g>
                </svg>
            </button>
            -->
        </form>
        
        <div class="flex gap-xxxxs">
            <button class="reset app-ui__header-btn js-tab-focus" aria-controls="notifications-popover">
                <svg class="icon" viewBox="0 0 20 20">
                    <title>Notifications</title>
                    <path d="M16,12V7a6,6,0,0,0-6-6h0A6,6,0,0,0,4,7v5L2,16H18Z" fill="none" stroke="currentColor" stroke-miterlimit="10" stroke-width="2" />
                    <path d="M7.184,18a2.982,2.982,0,0,0,5.632,0Z" />
                </svg>

                <span class="app-ui__notification-indicator"><i class="sr-only">You have 6 notifications</i></span>
            </button>
            
            <div class="dropdown inline-block js-dropdown">
                <div class="dropdown__wrapper">
                    <a class="app-ui__user-btn js-dropdown__trigger js-tab-focus" href="#">
                        <img src="/public/img/default-avatar.png" alt="avatar">
                    </a>
                    
                    <ul class="dropdown__menu js-dropdown__menu" aria-label="dropdown">
                        <li>
                            <a class="dropdown__item" href="/user/profile">Profile</a>
                        </li>
                        
                        <hr class="dropdown__separator" role="separator">
                        
                        <li>
                            <a class="dropdown__item" href="/home/Logout">Log Out</a>
                        </li>
                    </ul>
                </div>
            </div>
        </div>
    </div>
</header>
