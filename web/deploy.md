# llamaware web deployment

deployment guide for the llamaware enterprise platform showcase website featuring all 16 enterprise features and cross-platform compatibility.

## vercel deployment (recommended)

### one-click deploy
[![Deploy with Vercel](https://vercel.com/button)](https://vercel.com/new/clone?repository-url=https://github.com/bniladridas/Llamaware/tree/trunk/web)

### manual deploy
```bash
npm i -g vercel
vercel login
cd web
vercel
```

### github integration
1. push to github (trunk branch)
2. import at vercel.com
3. set root directory to `web`
4. configure build settings
5. deploy

## netlify deployment

### drag and drop
1. build locally: `cd web && npm run build`
2. drag `build` folder to netlify.com
3. configure custom domain

### git integration
1. connect github repository
2. set base directory: `web`
3. build command: `npm run build`
4. publish directory: `build`

## environment variables
```bash
REACT_APP_GOOGLE_ANALYTICS_ID=your_ga_id
REACT_APP_MIXPANEL_TOKEN=your_mixpanel_token
REACT_APP_GITHUB_REPO=bniladridas/Llamaware
REACT_APP_VERSION=2.1.0
```

## build settings

### vercel
- framework: create react app
- build command: `npm run build`
- output directory: `build`
- install command: `npm install`
- node version: 18.x

### netlify
- base directory: `web`
- build command: `npm run build`
- publish directory: `web/build`
- node version: 18

## performance features
- static asset caching with long-term headers
- gzip and brotli compression
- image optimization and lazy loading
- code splitting for enterprise features
- seo meta tags for all 16 features
- pwa manifest for offline support
- performance budgets and monitoring

## seo optimization
- structured data for software application
- open graph tags for social sharing
- twitter card meta tags
- canonical urls and sitemap
- performance score optimization
- accessibility compliance (wcag 2.1)

## cdn and global distribution
- automatic edge caching
- global cdn distribution
- http/2 and http/3 support
- ssl/tls encryption
- ddos protection

## monitoring and analytics
- core web vitals tracking
- user experience metrics
- conversion funnel analysis
- enterprise feature engagement
- cross-platform usage statistics