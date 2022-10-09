<?php
// Helper to allow ternary inside of string
$if = function($test, $true, $false){
    return ($test ? $true : $false);
};

// Helper to generate badge
$badge = function($type, $number){
    switch($type){
        case 'grey':
            return "<span class='sidenav__counter'>$number<i class='sr-only'>notifications</i></span>";
        case 'warning':
            return "<span class='sidenav__counter' style='background-color: hsla(var(--color-warning-h), var(--color-warning-s), var(--color-warning-l), var(--bg-o, 1));'>$number<i class='sr-only'>notifications</i></span>";
        case 'error':
            return "<span class='sidenav__counter' style='background-color: hsla(var(--color-error-h), var(--color-error-s), var(--color-error-l), var(--bg-o, 1));'>$number<i class='sr-only'>notifications</i></span>";
    }
};

/*
Layout

'Menu Name' => [
    'Icon' => 'Link to icon', 
    'CustomAfterIcon' => 'Display some html after icon and name'

    // Variant 1
    'SubMenus' => [
        'SubMenu1' => 'Link to page',
        'SubMenu2' => [
            'Link' => 'Link to page',
            'CustomAfter' => 'Display some html after name'
        ]
    ],
]
*/
$navigation = [
    /*'Movies' => [
        'Icon' => '
            <svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" fill="currentColor" class="bi bi-film" viewBox="0 0 16 16">
                <path d="M0 1a1 1 0 0 1 1-1h14a1 1 0 0 1 1 1v14a1 1 0 0 1-1 1H1a1 1 0 0 1-1-1V1zm4 0v6h8V1H4zm8 8H4v6h8V9zM1 1v2h2V1H1zm2 3H1v2h2V4zM1 7v2h2V7H1zm2 3H1v2h2v-2zm-2 3v2h2v-2H1zM15 1h-2v2h2V1zm-2 3v2h2V4h-2zm2 3h-2v2h2V7zm-2 3v2h2v-2h-2zm2 3h-2v2h2v-2z"/>
            </svg>
        ',
        'SubMenus' => [
            'Customers' => '/user/'
        ],
    ]*/
];

ksort($navigation);
?>

<style>
svg {
    margin-right: .3vw;
 }
</style>
<div class="app-ui__nav js-app-ui__nav" id="app-ui-navigation">
    <div class="flex flex-column height-100%">
        <div class="flex-grow overflow-auto momentum-scrolling">
            <!-- (mobile-only) search -->
            <div class="padding-x-md padding-top-md hide@md">
                <div class="search-input search-input--icon-right">
                    <input class="form-control width-100% height-100%" type="search" name="searchInputX" id="searchInputX" placeholder="Search..." aria-label="Search">
                    <button class="search-input__btn">
                        <svg class="icon" viewBox="0 0 24 24">
                            <title>Submit</title>
                            <g stroke-linecap="square" stroke-linejoin="miter" stroke-width="2" stroke="currentColor" fill="none" stroke-miterlimit="10">
                                <line x1="22" y1="22" x2="15.656" y2="15.656"></line>
                                <circle cx="10" cy="10" r="8"></circle>
                            </g>
                        </svg>
                    </button>
                </div>
            </div>
            
            <!-- side navigation -->
            <nav class="sidenav padding-y-sm js-sidenav">
                <div class="sidenav__label margin-bottom-xxxs">
                    <span class="text-sm color-contrast-medium text-xs@md">Main</span>
                </div>
                
                <ul class="sidenav__list">
                    <li class="sidenav__item">
                        <a href="/" class="sidenav__link" aria-current="page">
                            <svg class="icon sidenav__icon" aria-hidden="true" viewBox="0 0 16 16">
                                <g>
                                    <path d="M6,0H1C0.4,0,0,0.4,0,1v5c0,0.6,0.4,1,1,1h5c0.6,0,1-0.4,1-1V1C7,0.4,6.6,0,6,0z M5,5H2V2h3V5z"></path>
                                    <path d="M15,0h-5C9.4,0,9,0.4,9,1v5c0,0.6,0.4,1,1,1h5c0.6,0,1-0.4,1-1V1C16,0.4,15.6,0,15,0z M14,5h-3V2h3V5z"></path>
                                    <path d="M6,9H1c-0.6,0-1,0.4-1,1v5c0,0.6,0.4,1,1,1h5c0.6,0,1-0.4,1-1v-5C7,9.4,6.6,9,6,9z M5,14H2v-3h3V14z"></path>
                                    <path d="M15,9h-5c-0.6,0-1,0.4-1,1v5c0,0.6,0.4,1,1,1h5c0.6,0,1-0.4,1-1v-5C16,9.4,15.6,9,15,9z M14,14h-3v-3h3V14z"></path>
                                </g>
                            </svg>
                            <span class="sidenav__text text-sm@md">Home</span>
                        </a>
                    </li>

                    <?php
                        foreach($navigation as $key => $value){
                            $submenus = '';

                            ksort($value['SubMenus']);


                            foreach($value['SubMenus'] as $subkey => $subvalue){
                                if(is_array($subvalue)){
                                    $submenus .= "
                                        <li class='sidenav__item'>
                                            <a href='{$subvalue["Link"]}' class='sidenav__link'>
                                                <span class='sidenav__text text-sm@md'>$subkey</span>
                                                {$subvalue["CustomAfter"]}
                                            </a>
                                        </li>
                                    ";
                                }else{
                                    $submenus .= "
                                        <li class='sidenav__item'>
                                            <a href='{$subvalue}' class='sidenav__link'>
                                                <span class='sidenav__text text-sm@md'>$subkey</span>
                                            </a>
                                        </li>
                                    ";
                                }
                            }

                            if(!array_key_exists('CustomAfterIcon', $value)){
                                $value['CustomAfterIcon'] = '';
                            }


                            print("
                                <li class='sidenav__item'>
                                    <a href='#' class='sidenav__link js-sidenav__sublist-control'>
                                        {$value['Icon']}
                                        <span class='sidenav__text text-sm@md'>$key</span>
                                        {$value['CustomAfterIcon']}
                                    </a>

                                    <button class='reset sidenav__sublist-control js-sidenav__sublist-control js-tab-focus' aria-label='Toggle sub navigation'>
                                        <svg class='icon' viewBox='0 0 12 12'><polygon points='4 3 8 6 4 9 4 3' /></svg>
                                    </button>

                                    <ul class='sidenav__list'>
                                        $submenus
                                    </ul>
                                </li>
                            ");
                        }
                    ?>

                </ul>
                                    
                <!-- end side nav list -->
                
                <div class="sidenav__divider margin-y-xs" role="presentation"></div>
                
                <div class="sidenav__label margin-bottom-xxxs">
                    <span class="text-sm color-contrast-medium text-xs@md">Other</span>
                </div>
                
                <ul class="sidenav__list">
                    <li class="sidenav__item">
                        <a href="settings.html" class="sidenav__link">
                            <svg class="icon sidenav__icon" aria-hidden="true" viewBox="0 0 16 16">
                                <g>
                                    <circle cx="6" cy="8" r="2"></circle>
                                    <path d="M10,2H6C2.7,2,0,4.7,0,8s2.7,6,6,6h4c3.3,0,6-2.7,6-6S13.3,2,10,2z M10,12H6c-2.2,0-4-1.8-4-4s1.8-4,4-4h4 c2.2,0,4,1.8,4,4S12.2,12,10,12z"></path>
                                </g>
                            </svg>
                            <span class="sidenav__text text-sm@md">Settings</span>
                        </a>
                    </li>
                    
                    <li class="sidenav__item">
                        <a href="notifications.html" class="sidenav__link">
                            <svg class="icon sidenav__icon" aria-hidden="true" viewBox="0 0 16 16">
                                <g>
                                    <path d="M10,14H6c0,1.1,0.9,2,2,2S10,15.1,10,14z"></path> 
                                    <path d="M15,11h-0.5C13.8,10.3,13,9.3,13,8V5c0-2.8-2.2-5-5-5S3,2.2,3,5v3c0,1.3-0.8,2.3-1.5,3H1c-0.6,0-1,0.4-1,1 s0.4,1,1,1h14c0.6,0,1-0.4,1-1S15.6,11,15,11z"></path>
                                </g>
                            </svg>
                            
                            <span class="sidenav__text text-sm@md">Notifications</span>
                            <span class="sidenav__counter">23 <i class="sr-only">notifications</i></span>
                        </a>
                    </li>
                </ul>
            </nav>
        </div>
      
        <!-- light/dark mode toggle -->
        <div class="padding-md padding-sm@md margin-top-auto flex-shrink-0 border-top border-contrast-lower ie:hide">
            <div class="flex items-center justify-between@md">
                <p class="text-sm@md">Dark Mode</p>
                
                <div class="switch dark-mode-switch margin-left-xxs">
                    <input class="switch__input" type="checkbox" id="switch-light-dark">
                    <label aria-hidden="true" class="switch__label" for="switch-light-dark">On</label>
                    <div aria-hidden="true" class="switch__marker"></div>
                </div>
            </div>
        </div>
    </div>
</div>
